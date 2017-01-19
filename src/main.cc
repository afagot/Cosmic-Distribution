//******************************************************
//*      Cosmic Muons Monte Carlo Simulation           *
//*                                                    *
//*      Alexis FAGOT                                  *
//*      alexis.fagot@ugent.be                         *
//*                                                    *
//*      Janvier 16, 2016                              *
//*                                                    *
//*      HEADER FILE                                   *
//******************************************************


#include "../include/fonctions.h"
#include <sstream>

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"

using namespace std;

int main(int argc ,char *argv[]){
    stringstream converter;
    converter << argv[0];
    string program;
    converter >> program;
    converter.clear();

    if(argc != 3){
        cout << "Expects to have 3 parameters\n";
        cout << "[Offline] USAGE is : " << program << " Hoffset_low Hoffset_high\n";
        return -1;
    } else if(argc == 3){
        int Hoffset_low = 0;
        int Hoffset_high = 0;

        converter << argv[1];
        converter >> Hoffset_low;
        converter.clear();

        converter << argv[2];
        converter >> Hoffset_high;
        converter.clear();

        for(int h=Hoffset_low; h<=Hoffset_high; h++){
            for(int d=0; d<int(__scint_D); d++){
                string ROOTName = "Angle-dependency_" + intTostring(h) + "_" + intTostring(d) + ".root";

                TFile *ROOTFile = new TFile(ROOTName.c_str(),"recreate");

                //Write results into a csv file

                string CSVName = "Angle-dependency_" + intTostring(h) + "_" + intTostring(d) + ".csv";
                ofstream CSVFile(CSVName.c_str(), ios::out);

                for(unsigned int a=0; a<90; a+=5){

                    //definition of the scintillator inclination angle
                    double alpha = a*PI/180.;

                    /** PROGRAM CODE **/
                    // Start with random seed
                    random_device rd;
                    Generator generator(rd());

                    // Get the thing started
                    generator.discard(100);

                    //Definitions of the coordinates and of the cosmic direction
                    Point muonPos, GroundPos, ScreenPos;
                    Direction direction;

                    //List of root IDs and root Titles for all triggered muons
                    string grd_profilID = "grd_prof_" + intTostring(a);
                    string grd_profilIDX = "grd_prof_X_" + intTostring(a);
                    string grd_profilIDY = "grd_prof_Y_" + intTostring(a);
                    string grd_profilTitle = "Muon hit profil on the ground";
                    string scr_profilID = "scr_prof_" + intTostring(a);
                    string scr_profilIDX = "scr_prof_X_" + intTostring(a);
                    string scr_profilIDZ = "scr_prof_Z_" + intTostring(a);
                    string scr_profilTitle = "Muon hit profil on the vertical screen";
                    string muondirID = "direction_" + intTostring(a);
                    string muondirIDT = "direction_theta_" + intTostring(a);
                    string muondirIDP = "direction_phi_" + intTostring(a);
                    string muondirTitle = "Triggered muon direction components";

                    TH2F* GroundProfil = new TH2F(grd_profilID.c_str(),grd_profilTitle.c_str(),40,-100,100,40,-100,100);
                    TH1F* GroundProfilX = new TH1F(grd_profilIDX.c_str(),grd_profilTitle.c_str(),40,-100,100);
                    TH1F* GroundProfilY = new TH1F(grd_profilIDY.c_str(),grd_profilTitle.c_str(),40,-100,100);
                    TH2F* ScreenProfil = new TH2F(scr_profilID.c_str(),scr_profilTitle.c_str(),40,-100,100,40,-100+h,100+h);
                    TH1F* ScreenProfilX = new TH1F(scr_profilIDX.c_str(),grd_profilTitle.c_str(),40,-100,100);
                    TH1F* ScreenProfilZ = new TH1F(scr_profilIDZ.c_str(),grd_profilTitle.c_str(),40,-100+h,100+h);
                    TH2F* MuonDir = new TH2F(muondirID.c_str(),muondirTitle.c_str(),10,0.,90.,40,0.,360.);
                    TH1F* MuonDirT = new TH1F(muondirIDT.c_str(),muondirTitle.c_str(),10,0.,90.);
                    TH1F* MuonDirP = new TH1F(muondirIDP.c_str(),muondirTitle.c_str(),40,0.,360.);

                    //List of root IDs and root Titles for triggered forward muons
                    string grd_profilIDF = "grd_prof_f_" + intTostring(a);
                    string grd_profilIDXF = "grd_prof_X_f_" + intTostring(a);
                    string grd_profilIDYF = "grd_prof_Y_f_" + intTostring(a);
                    string grd_profilTitleF = "Forward muon hit profil on the ground";
                    string scr_profilIDF = "scr_prof_f_" + intTostring(a);
                    string scr_profilIDXF = "scr_prof_X_f_" + intTostring(a);
                    string scr_profilIDZF = "scr_prof_Z_f_" + intTostring(a);
                    string scr_profilTitleF = "Forward muon hit profil on the vertical screen";
                    string muondirIDF = "direction_f_" + intTostring(a);
                    string muondirIDTF = "direction_theta_f_" + intTostring(a);
                    string muondirIDPF = "direction_phi_f_" + intTostring(a);
                    string muondirTitleF = "Triggered forward muon direction components";

                    TH2F* GroundProfilF = new TH2F(grd_profilIDF.c_str(),grd_profilTitleF.c_str(),40,-100,100,40,-100,100);
                    TH1F* GroundProfilXF = new TH1F(grd_profilIDXF.c_str(),grd_profilTitleF.c_str(),40,-100,100);
                    TH1F* GroundProfilYF = new TH1F(grd_profilIDYF.c_str(),grd_profilTitleF.c_str(),40,-100,100);
                    TH2F* ScreenProfilF = new TH2F(scr_profilIDF.c_str(),scr_profilTitleF.c_str(),40,-100,100,40,-100+h,100+h);
                    TH1F* ScreenProfilXF = new TH1F(scr_profilIDXF.c_str(),grd_profilTitleF.c_str(),40,-100,100);
                    TH1F* ScreenProfilZF = new TH1F(scr_profilIDZF.c_str(),grd_profilTitleF.c_str(),40,-100+h,100+h);
                    TH2F* MuonDirF = new TH2F(muondirIDF.c_str(),muondirTitleF.c_str(),10,0.,90.,40,0.,360.);
                    TH1F* MuonDirTF = new TH1F(muondirIDTF.c_str(),muondirTitleF.c_str(),10,0.,90.);
                    TH1F* MuonDirPF = new TH1F(muondirIDPF.c_str(),muondirTitleF.c_str(),40,0.,360.);

                    //List of root IDs and root Titles for triggered backward muons
                    string grd_profilIDB = "grd_prof_b_" + intTostring(a);
                    string grd_profilIDXB = "grd_prof_X_b_" + intTostring(a);
                    string grd_profilIDYB = "grd_prof_Y_b_" + intTostring(a);
                    string grd_profilTitleB = "Backward muon hit profil on the ground";
                    string scr_profilIDB = "scr_prof_b_" + intTostring(a);
                    string scr_profilIDXB = "scr_prof_X_b_" + intTostring(a);
                    string scr_profilIDZB = "scr_prof_Z_b_" + intTostring(a);
                    string scr_profilTitleB = "Backward muon hit profil on the vertical screen";
                    string muondirIDB = "direction_b_" + intTostring(a);
                    string muondirIDTB = "direction_theta_b_" + intTostring(a);
                    string muondirIDPB = "direction_phi_b_" + intTostring(a);
                    string muondirTitleB = "Triggered backward muon direction components";

                    TH2F* GroundProfilB = new TH2F(grd_profilIDB.c_str(),grd_profilTitleB.c_str(),40,-100,100,40,-100,100);
                    TH1F* GroundProfilXB = new TH1F(grd_profilIDXB.c_str(),grd_profilTitleB.c_str(),40,-100,100);
                    TH1F* GroundProfilYB = new TH1F(grd_profilIDYB.c_str(),grd_profilTitleB.c_str(),40,-100,100);
                    TH2F* ScreenProfilB = new TH2F(scr_profilIDB.c_str(),scr_profilTitleB.c_str(),40,-100,100,40,-100+h,100+h);
                    TH1F* ScreenProfilXB = new TH1F(scr_profilIDXB.c_str(),grd_profilTitleB.c_str(),40,-100,100);
                    TH1F* ScreenProfilZB = new TH1F(scr_profilIDZB.c_str(),grd_profilTitleB.c_str(),40,-100+h,100+h);
                    TH2F* MuonDirB = new TH2F(muondirIDB.c_str(),muondirTitleB.c_str(),10,0.,90.,40,0.,360.);
                    TH1F* MuonDirTB = new TH1F(muondirIDTB.c_str(),muondirTitleB.c_str(),10,0.,90.);
                    TH1F* MuonDirPB = new TH1F(muondirIDPB.c_str(),muondirTitleB.c_str(),40,0.,360.);

                    unsigned int Nmuons = 1e4;  //The number of muons to generate
                    float Stat = float(Nmuons); //Number used for the progress bar only - is equal to Nmuons
                    float progress = 0.0;       //Estimation of the progress in percent for each threshold value

                    unsigned int n = 0;

                    //Height of the muon generation plane
                    //this plane is placed at the level of the height of the barycenter
                    //of the 2 scintillator surfaces, D being the distance in between them
                    double scint_D = double(d);
                    double muon_H = __muon_H + double(h);

                    while(n<Nmuons){
                        muonPos = getRandomMuonPosition(generator,muon_H);
                        direction = getRandomDirection(generator);

                        bool in_top_scint = isInScint(muonPos,direction,alpha,scint_D,muon_H,"TOP");
                        bool in_bottom_scint = isInScint(muonPos,direction,alpha,scint_D,muon_H,"BOTTOM");

                        bool in_trigger = in_top_scint && in_bottom_scint;

                        if (in_trigger){
                            GroundPos = getHitPosition(muonPos,direction,alpha,scint_D,muon_H,"GROUND");
                            GroundProfil->Fill(GroundPos.x,GroundPos.y);
                            GroundProfilX->Fill(GroundPos.x);
                            GroundProfilY->Fill(GroundPos.y);

                            ScreenPos = getHitPosition(muonPos,direction,alpha,scint_D,muon_H,"SCREEN");
                            ScreenProfil->Fill(ScreenPos.x,ScreenPos.z);
                            ScreenProfilX->Fill(ScreenPos.x);
                            ScreenProfilZ->Fill(ScreenPos.z);

                            MuonDir->Fill(direction.first*180./PI, direction.second*180./PI);
                            MuonDirT->Fill(direction.first*180./PI);
                            MuonDirP->Fill(direction.second*180./PI);

                            if(direction.second < PI){
                                GroundProfilF->Fill(GroundPos.x,GroundPos.y);
                                GroundProfilXF->Fill(GroundPos.x);
                                GroundProfilYF->Fill(GroundPos.y);
                                ScreenProfilF->Fill(ScreenPos.x,ScreenPos.z);
                                ScreenProfilXF->Fill(ScreenPos.x);
                                ScreenProfilZF->Fill(ScreenPos.z);
                                MuonDirF->Fill(direction.first*180./PI, direction.second*180./PI);
                                MuonDirTF->Fill(direction.first*180./PI);
                                MuonDirPF->Fill(direction.second*180./PI);
                            } else {
                                GroundProfilB->Fill(GroundPos.x,GroundPos.y);
                                GroundProfilXB->Fill(GroundPos.x);
                                GroundProfilYB->Fill(GroundPos.y);
                                ScreenProfilB->Fill(ScreenPos.x,ScreenPos.z);
                                ScreenProfilXB->Fill(ScreenPos.x);
                                ScreenProfilZB->Fill(ScreenPos.z);
                                MuonDirB->Fill(direction.first*180./PI, direction.second*180./PI);
                                MuonDirTB->Fill(direction.first*180./PI);
                                MuonDirPB->Fill(direction.second*180./PI);
                            }

                            n++;
                        }

                        //Print progress bar
                        progress = float(n/Stat);

                        int barWidth = 25;

                        cout << "Progress " << ROOTName << " a-" << intTostring(a);
                        cout << " [";
                        int pos = barWidth * progress;
                        for (int i = 0; i < barWidth; ++i) {
                            if (i < pos) cout << "=";
                            else if (i == pos) cout << ">";
                            else cout << " ";
                        }
                        cout << "] " << int(progress * 100.0) << " %\r";
                        cout.flush();
                    }
                    cout << endl;

                    //Get ground variables

                    double GroundMeanF = GroundProfilYF->GetMean();
                    double GroundRMSF = GroundProfilYF->GetRMS();
                    double GroundMaxF = GroundProfilYF->GetMaximum();
                    double GroundIntF = GroundProfilYF->Integral();

                    double GroundMeanB = GroundProfilYB->GetMean();
                    double GroundRMSB = GroundProfilYB->GetRMS();
                    double GroundMaxB = GroundProfilYB->GetMaximum();
                    double GroundIntB = GroundProfilYB->Integral();

                    //Calculate other results

                    double GroundPeakDist = abs(GroundMeanF-GroundMeanB);
                    double GroundPeakRatio = GroundMaxB/GroundMaxF;
                    double GroundIntRatio = GroundIntB/GroundIntF;

                    //Get screen variables

                    double ScreenMeanF = ScreenProfilZF->GetMean();
                    double ScreenRMSF = ScreenProfilZF->GetRMS();
                    double ScreenMaxF = ScreenProfilZF->GetMaximum();
                    double ScreenIntF = ScreenProfilZF->Integral();

                    double ScreenMeanB = ScreenProfilZB->GetMean();
                    double ScreenRMSB = ScreenProfilZB->GetRMS();
                    double ScreenMaxB = ScreenProfilZB->GetMaximum();
                    double ScreenIntB = ScreenProfilZB->Integral();

                    //Calculate other results

                    double ScreenPeakDist = abs(ScreenMeanF-ScreenMeanB);
                    double ScreenPeakRatio = ScreenMaxB/ScreenMaxF;
                    double ScreenIntRatio = ScreenIntB/ScreenIntF;

                    //Finally write in the CSV file...

                    CSVFile << intTostring(h) << '\t'
                            << intTostring(d) << '\t'
                            << intTostring(a) << '\t'
                            << GroundMeanF << '\t'
                            << GroundRMSF << '\t'
                            << GroundMaxF << '\t'
                            << GroundIntF << '\t'
                            << GroundMeanB << '\t'
                            << GroundRMSB << '\t'
                            << GroundMaxB << '\t'
                            << GroundIntB << '\t'
                            << GroundPeakDist << '\t'
                            << GroundPeakRatio << '\t'
                            << GroundIntRatio << '\t'
                            << ScreenMeanF << '\t'
                            << ScreenRMSF << '\t'
                            << ScreenMaxF << '\t'
                            << ScreenIntF << '\t'
                            << ScreenMeanB << '\t'
                            << ScreenRMSB << '\t'
                            << ScreenMaxB << '\t'
                            << ScreenIntB << '\t'
                            << ScreenPeakDist << '\t'
                            << ScreenPeakRatio << '\t'
                            << ScreenIntRatio << '\n';

                    //Write histos

                    GroundProfil->Write();
                    GroundProfilX->Write();
                    GroundProfilY->Write();
                    ScreenProfil->Write();
                    ScreenProfilX->Write();
                    ScreenProfilZ->Write();
                    MuonDir->Write();
                    MuonDirT->Write();
                    MuonDirP->Write();

                    GroundProfilF->Write();
                    GroundProfilXF->Write();
                    GroundProfilYF->Write();
                    ScreenProfilF->Write();
                    ScreenProfilXF->Write();
                    ScreenProfilZF->Write();
                    MuonDirF->Write();
                    MuonDirTF->Write();
                    MuonDirPF->Write();

                    GroundProfilB->Write();
                    GroundProfilXB->Write();
                    GroundProfilYB->Write();
                    ScreenProfilB->Write();
                    ScreenProfilXB->Write();
                    ScreenProfilZB->Write();
                    MuonDirB->Write();
                    MuonDirTB->Write();
                    MuonDirPB->Write();
                }
                CSVFile.close();
                ROOTFile->Close();
            }
        }
    }
    return 0;
}
