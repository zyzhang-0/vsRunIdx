//const int nRun = 115;
#include "prodRunId.h"
int nRun = sizeof(runId)/sizeof(TString);
//24168003-24168009 prod
//24168021-24168029 zerofield
//24168032-end prod
TFile* ftmp;
TH1F* hFracVsApvIdx[3];
TH2F* hFracVsRunIdxVsApvIdx[3];
TH1F* hFracVsRunIdxTMP[8][3];

TH1F* hMeanVsApvIdx;
TH2F* hMeanVsRunIdxVsApvIdx;
TH1F* hMeanVsRunIdxTMP[8];

TH1F* hNVsApvIdx;
TH2F* hNVsRunIdxVsApvIdx;
TH1F* hNVsRunIdxTMP[8];

TCanvas* ctmp;
TLegend* ltmp;

TF1* fFrac;
void GetFracVsRunIdx_3tb_simple(){
    gStyle->SetOptStat(0);
    gROOT->SetBatch(kTRUE);
    
    fFrac = new TF1("fFrac", "[0]", -0.5, nRun-0.5);
    for(int i = 0 ; i < 3 ; i++){
        hFracVsRunIdxVsApvIdx[i] = new TH2F(Form("hFracVsRunIdxVsApvIdx_%d", i), Form("Frac of bin %d Vs RunIdx (Y) Vs ApvIdx (X)", i), 288, -0.5, 287.5, nRun, -0.5, nRun-0.5);
        hFracVsRunIdxVsApvIdx[i]->GetXaxis()->SetTitle("Global APV Idx (0-287)");
        hFracVsRunIdxVsApvIdx[i]->GetYaxis()->SetTitle(Form("Run Idx (0-%d)", nRun-1));
    }
    hMeanVsRunIdxVsApvIdx = new TH2F("hMeanVsRunIdxVsApvIdx", "Mean Vs RunIdx (Y) Vs ApvIdx (X)", 288, -0.5, 287.5, nRun, -0.5, nRun-0.5);
    hNVsRunIdxVsApvIdx = new TH2F("hNVsRunIdxVsApvIdx", "N Vs RunIdx (Y) Vs ApvIdx (X)", 288, -0.5, 287.5, nRun, -0.5, nRun-0.5);
    
    
    for(int irun = 0 ; irun < nRun ; irun++){
        ftmp = new TFile(Form("RootFiles/%s.root", runId[irun].Data()), "read");
        for(int itb = 0 ; itb < 3 ; itb++){
            hFracVsApvIdx[itb] = (TH1F*)ftmp->Get(Form("hFracVsApvIdx_%d", itb));
            for(int iapv = 0 ; iapv < 288 ; iapv++){
                double content = hFracVsApvIdx[itb]->GetBinContent(iapv+1);
                double err = hFracVsApvIdx[itb]->GetBinError(iapv+1);
                hFracVsRunIdxVsApvIdx[itb]->SetBinContent(iapv+1, irun+1, content);
                hFracVsRunIdxVsApvIdx[itb]->SetBinError(iapv+1, irun+1, err);
            }
        }
        hMeanVsApvIdx = (TH1F*)ftmp->Get("hMeanVsApvIdx");
        for(int iapv = 0 ; iapv < 288 ; iapv++){
            double content = hMeanVsApvIdx->GetBinContent(iapv+1);
            double err = hMeanVsApvIdx->GetBinError(iapv+1);
            hMeanVsRunIdxVsApvIdx->SetBinContent(iapv+1, irun+1, content);
            hMeanVsRunIdxVsApvIdx->SetBinError(iapv+1, irun+1, err);
        }
        hNVsApvIdx = (TH1F*)ftmp->Get("hNVsApvIdx");
        for(int iapv = 0 ; iapv < 288 ; iapv++){
            double content = hNVsApvIdx->GetBinContent(iapv+1);
            double err = hNVsApvIdx->GetBinError(iapv+1);
            hNVsRunIdxVsApvIdx->SetBinContent(iapv+1, irun+1, content);
            hNVsRunIdxVsApvIdx->SetBinError(iapv+1, irun+1, err);
        }
        ftmp->Close();
        delete ftmp;
    }
    
    ctmp = new TCanvas("ctmp", "ctmp", 1600, 900);
    ctmp->SetFillColor(kGray);
    ctmp->Divide(3,2, 1e-3, 1e-3);
    
    for(int itb = 0 ; itb < 3 ; itb++){
        ctmp->cd(itb+1);
        hFracVsRunIdxVsApvIdx[itb]->DrawClone("colz");
    }
    ctmp->cd(4);
    hMeanVsRunIdxVsApvIdx->Draw("colz");
    ctmp->cd(5);
    //hNVsRunIdxVsApvIdx->GetYaxis()->SetRangeUser(94.5, 97.5);
    hNVsRunIdxVsApvIdx->Draw("colz");
    ctmp->Print("plots/3tb.pdf(");
    
    
    
    ltmp = new TLegend(0.1, 0.1, 0.9, 0.9);
    for(int rdo = 0 ; rdo < 6 ; rdo++){
        for(int arm = 0 ; arm < 3 ; arm++){
            for(int group = 0 ; group < 2 ; group++){
                double max = 0.;
                double min = 1.;
                double avgFrac[8][3]={0.};
                double errFrac[8][3]={0.};
                for(int itb = 0 ; itb < 3 ; itb++){
                    ctmp->cd(itb+1);
                    max = 0.;
                    min = 1.;
                    for(int apv = 0 ; apv < 8 ; apv++){
                        int iapv = apv + group*8 + arm*2*8 + rdo*2*8*3;
                        hFracVsRunIdxTMP[apv][itb] = (TH1F*)hFracVsRunIdxVsApvIdx[itb]->ProjectionY(Form("hFracVsRunIdxTMP_%d_%d", apv, itb), iapv+1, iapv+1, "e");
                        hFracVsRunIdxTMP[apv][itb]->SetTitle(Form("tb%d Frac of RDO%d_ARM%d_GROUP%d_APVx", itb, rdo+1, arm, group));
                        for(int irun = 0 ; irun < nRun ; irun++){
                            double content = hFracVsRunIdxTMP[apv][itb]->GetBinContent(irun+1);
                            double err = hFracVsRunIdxTMP[apv][itb]->GetBinError(irun+1);
                            max = ((content+err)>max)?(content+err):max;
                            if((content!=0||err!=0)) min = ((content-err)<min)?(content-err):min;
                        }
                    }
                    max = 1.0;
                    min = 0.;
                    for(int apv = 0 ; apv < 8 ; apv++){
                        hFracVsRunIdxTMP[apv][itb]->SetMarkerColor(apv+2);
                        hFracVsRunIdxTMP[apv][itb]->SetLineColor(apv+2);
                        hFracVsRunIdxTMP[apv][itb]->GetYaxis()->SetRangeUser(min, max);
                        gPad->SetGridy();
                        hFracVsRunIdxTMP[apv][itb]->DrawClone((apv==0)?"histe":"histesame");
                        if(apv==0){
                            hFracVsRunIdxTMP[apv][itb]->DrawClone("histesame");
                        }
                        //cout<<Form("RDO%d_ARM%d_GROUP%d_APV%d_TB%d", rdo+1, arm, group, apv, itb)<<endl;
                        hFracVsRunIdxTMP[apv][itb]->Fit(fFrac, "ME0Q", "", -0.5, nRun-0.5);
                        avgFrac[apv][itb] = fFrac->GetParameter(0);
                        errFrac[apv][itb] = fFrac->GetParError(0);
                    }
                }
                for(int apv = 0 ; apv < 8 ; apv++){
                    if(avgFrac[apv][0]-errFrac[apv][0]*5 > avgFrac[apv][1]+errFrac[apv][1]*5) cout<<Form("RDO%d_ARM%d_GROUP%d_APV%d Bin 0 significantly higher than Bin 1: %f-%f > %f+%f", rdo+1, arm, group, apv, avgFrac[apv][0], errFrac[apv][0]*5, avgFrac[apv][1], errFrac[apv][1]*5)<<endl;
                    if(avgFrac[apv][2]-errFrac[apv][2]*5 > avgFrac[apv][1]+errFrac[apv][1]*5) cout<<Form("RDO%d_ARM%d_GROUP%d_APV%d Bin 2 significantly higher than Bin 1: %f-%f > %f+%f", rdo+1, arm, group, apv, avgFrac[apv][2], errFrac[apv][2]*5, avgFrac[apv][1], errFrac[apv][1]*5)<<endl;
                }
                
                //==========
                ctmp->cd(4);
                max = 0.;
                min = 3.;
                for(int apv = 0 ; apv < 8 ; apv++){
                    int iapv = apv + group*8 + arm*2*8 + rdo*2*8*3;
                    hMeanVsRunIdxTMP[apv] = (TH1F*)hMeanVsRunIdxVsApvIdx->ProjectionY(Form("hMeanVsRunIdxTMP_%d", apv), iapv+1, iapv+1, "e");
                    hMeanVsRunIdxTMP[apv]->SetTitle(Form("Mean of RDO%d_ARM%d_GROUP%d_APVx", rdo+1, arm, group));
                    for(int irun = 0 ; irun < nRun ; irun++){
                        double content = hMeanVsRunIdxTMP[apv]->GetBinContent(irun+1);
                        double err = hMeanVsRunIdxTMP[apv]->GetBinError(irun+1);
                        max = ((content+err)>max)?(content+err):max;
                        if((content!=0||err!=0)) min = ((content-err)<min)?(content-err):min;
                    }
                }
                max = 3.0;
                min = 0.;
                for(int apv = 0 ; apv < 8 ; apv++){
                    hMeanVsRunIdxTMP[apv]->SetMarkerColor(apv+2);
                    hMeanVsRunIdxTMP[apv]->SetLineColor(apv+2);
                    hMeanVsRunIdxTMP[apv]->GetYaxis()->SetRangeUser(min, max);
                    gPad->SetGridy();
                    hMeanVsRunIdxTMP[apv]->DrawClone((apv==0)?"histe":"histesame");
                    if(apv==0){
                        hMeanVsRunIdxTMP[apv]->DrawClone("histesame");
                    }
                    
                }
                //==========
                ctmp->cd(5);
                max = 0.;
                min = 1e10;
                for(int apv = 0 ; apv < 8 ; apv++){
                    int iapv = apv + group*8 + arm*2*8 + rdo*2*8*3;
                    hNVsRunIdxTMP[apv] = (TH1F*)hNVsRunIdxVsApvIdx->ProjectionY(Form("hNVsRunIdxTMP_%d", apv), iapv+1, iapv+1, "e");
                    hNVsRunIdxTMP[apv]->SetTitle(Form("Total Entreis of RDO%d_ARM%d_GROUP%d_APVx", rdo+1, arm, group));
                    for(int irun = 0 ; irun < nRun ; irun++){
                        double content = hNVsRunIdxTMP[apv]->GetBinContent(irun+1);
                        double err = hNVsRunIdxTMP[apv]->GetBinError(irun+1);
                        max = ((content+err)>max)?(content+err):max;
                        if((content!=0||err!=0)) min = ((content-err)<min)?(content-err):min;
                    }
                }
                //max = 1.0;
                //min = 0.;
                for(int apv = 0 ; apv < 8 ; apv++){
                    hNVsRunIdxTMP[apv]->SetMarkerColor(apv+2);
                    hNVsRunIdxTMP[apv]->SetLineColor(apv+2);
                    hNVsRunIdxTMP[apv]->GetYaxis()->SetRangeUser(min, max);
                    gPad->SetGridy();
                    hNVsRunIdxTMP[apv]->DrawClone((apv==0)?"histe":"histesame");
                    if(apv==0){
                        hNVsRunIdxTMP[apv]->DrawClone("histesame");
                    }
                    if(rdo==0&&arm==0&&group==0){
                        ltmp->AddEntry(hNVsRunIdxTMP[apv], Form("x = %d", apv), "lpe");
                    }
                    
                }
                if(rdo==0&&arm==0&&group==0){
                    ctmp->cd(6);
                    ltmp->Draw();
                }
                ctmp->Print((rdo==5&&arm==2&&group==1)?"plots/3tb.pdf)":"plots/3tb.pdf");
            }
        }
    }
}
