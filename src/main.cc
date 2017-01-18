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
#include "TH2F.h"

using namespace std;

int main(int argc ,char *argv[]){
    stringstream converter;
    converter << argv[0];
    string program;
    converter >> program;
    converter.clear();

    if(argc != 5){
        cout << "Expects to have 3 parameters\n";
        cout << "[Offline] USAGE is : " << program << " Doffset_low Doffset_high Soffset_low Soffset_high/n";
        return -1;
    } else if(argc == 3){
        int Doffset_low = 0;
        int Doffset_high = 0;
        int Soffset_low = 0;
        int Soffset_high = 0;

        converter << argv[1];
        converter >> Doffset_low;
        converter.clear();

        converter << argv[2];
        converter >> Doffset_high;
        converter.clear();

        converter << argv[3];
        converter >> Soffset_low;
        converter.clear();

        converter << argv[4];
        converter >> Soffset_high;
        converter.clear();

        for(int s=Soffset_low; s<=Soffset_high; s++){
            for(int d=Doffset_low; d<=Doffset_high; d++){
                string ROOTName = "Angle-dependency_" + intTostring(s) + "_" + intTostring(d) + ".root";

                TFile *outputFile = new TFile(ROOTName.c_str(),"recreate");

                for(unsigned int a=0; a<60; a+=5){

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
                    string grd_profilTitle = "Muon hit profil on the ground";
                    string scr_profilID = "scr_prof_" + intTostring(a);
                    string scr_profilTitle = "Muon hit profil on the vertical screen";
                    string muondirID = "direction_" + intTostring(a);
                    string muondirTitle = "Triggered muon direction components";

                    TH2F* GroundProfil = new TH2F(grd_profilID.c_str(),grd_profilTitle.c_str(),200,-100,100,200,-100,100);
                    TH2F* ScreenProfil = new TH2F(scr_profilID.c_str(),scr_profilTitle.c_str(),200,-100,100,200,-100,100);
                    TH2F* MuonDir = new TH2F(muondirID.c_str(),muondirTitle.c_str(),90,0.,90.,40,0.,360.);

                    //List of root IDs and root Titles for triggered forward muons
                    string grd_profilIDF = "grd_prof_f_" + intTostring(a);
                    string grd_profilTitleF = "Forward muon hit profil on the ground";
                    string scr_profilIDF = "scr_prof_f_" + intTostring(a);
                    string scr_profilTitleF = "Forward muon hit profil on the vertical screen";
                    string muondirIDF = "direction_f_" + intTostring(a);
                    string muondirTitleF = "Triggered forward muon direction components";

                    TH2F* GroundProfilF = new TH2F(grd_profilIDF.c_str(),grd_profilTitleF.c_str(),200,-100,100,200,-100,100);
                    TH2F* ScreenProfilF = new TH2F(scr_profilIDF.c_str(),scr_profilTitleF.c_str(),200,-100,100,200,-100,100);
                    TH2F* MuonDirF = new TH2F(muondirIDF.c_str(),muondirTitleF.c_str(),90,0.,90.,40,0.,360.);

                    //List of root IDs and root Titles for triggered backward muons
                    string grd_profilIDB = "grd_prof_b_" + intTostring(a);
                    string grd_profilTitleB = "Backward muon hit profil on the ground";
                    string scr_profilIDB = "scr_prof_b_" + intTostring(a);
                    string scr_profilTitleB = "Backward muon hit profil on the vertical screen";
                    string muondirIDB = "direction_b_" + intTostring(a);
                    string muondirTitleB = "Triggered backward muon direction components";

                    TH2F* GroundProfilB = new TH2F(grd_profilIDB.c_str(),grd_profilTitleB.c_str(),200,-100,100,200,-100,100);
                    TH2F* ScreenProfilB = new TH2F(scr_profilIDB.c_str(),scr_profilTitleB.c_str(),200,-100,100,200,-100,100);
                    TH2F* MuonDirB = new TH2F(muondirIDB.c_str(),muondirTitleB.c_str(),90,0.,90.,40,0.,360.);

                    unsigned int Nmuons = 1e3;  //The number of muons to generate
                    float Stat = float(Nmuons); //Number used for the progress bar only - is equal to Nmuons
                    float progress = 0.0;       //Estimation of the progress in percent for each threshold value

                    unsigned int n = 0;

                    while(n<Nmuons){
                        muonPos = getRandomMuonPosition(generator,__scint_H+s+__scint_W*sin(alpha)/2.);
                        direction = getRandomDirection(generator);

                        bool in_trigger = isInScint(muonPos,direction,alpha,d,s,"TOP") && isInScint(muonPos,direction,alpha,d,s,"BOTTOM");

                        if (in_trigger){
                            GroundPos = getHitPosition(muonPos,direction,alpha,d,s,"GROUND");
                            GroundProfil->Fill(GroundPos.x,GroundPos.y);

                            ScreenPos = getHitPosition(muonPos,direction,alpha,d,s,"SCREEN");
                            ScreenProfil->Fill(ScreenPos.x,ScreenPos.z);

                            MuonDir->Fill(direction.first*180./PI, direction.second*180./PI);

                            if(direction.second < PI){
                                GroundProfilF->Fill(GroundPos.x,GroundPos.y);
                                ScreenProfilF->Fill(ScreenPos.x,ScreenPos.z);
                                MuonDirF->Fill(direction.first*180./PI, direction.second*180./PI);
                            } else {
                                GroundProfilB->Fill(GroundPos.x,GroundPos.y);
                                ScreenProfilB->Fill(ScreenPos.x,ScreenPos.z);
                                MuonDirB->Fill(direction.first*180./PI, direction.second*180./PI);
                            }

                            n++;
                        }

                        //Print progress bar
                        progress = float(n/Stat);

                        int barWidth = 70;

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

                    //Write histos

                    GroundProfil->Write();
                    ScreenProfil->Write();
                    MuonDir->Write();

                    GroundProfilF->Write();
                    ScreenProfilF->Write();
                    MuonDirF->Write();

                    GroundProfilB->Write();
                    ScreenProfilB->Write();
                    MuonDirB->Write();

                    //Delete histos

                    delete GroundProfil;
                    delete ScreenProfil;
                    delete MuonDir;

                    delete GroundProfilF;
                    delete ScreenProfilF;
                    delete MuonDirF;

                    delete GroundProfilB;
                    delete ScreenProfilB;
                    delete MuonDirB;
                }
                outputFile->Close();
            }
        }
    }
    return 0;
}
