#include <string.h>
#include <iostream>
#include <fstream>
#include <graphics.h>
#include <winbgim.h>
#include <conio.h>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <math.h>
#include <windows.h>
#include <time.h>
#define INT_COLOR COLOR(224,122,95)
using namespace std;
fstream gout("data.in");
/*
    Reprezentare
1=jucator 1, 2=jucator 2, 3=neutru,poate fi completat cu conexiuni vertical sau orizontale,
4=conector vertical juc1, 5=conector orizontal juca1, 6=conector vertical juc2, 7=conector orizontal juc
    Ferestre
0=meniu
1=joc
2=optiuni
3=selector culori
4=selector dimensiune
5=tip joc
6=selector dificultate
7=help
8-extra
9-Vizualizatori Miscari
10-High score
    Limba
1-Romana
2-Englez
3-Franceza
*/
struct mutari
{
    int linie,coloana;
};
time_t tmp;
int a[200][200],b[200][200][200],n=8,vest,nord,diametru,jucatorCurent,contorCopiiMatrici=-1;
int castigator=0; //1 sau 2
int fereastraActiva=0;
int culoareJuc1=0; //0=BLACK 4=RED 15=WHITE
int culoareJuc2=4;
int counterMisc=-1;
int culoareTabla=15;
int culoareInterfata=0; //chenarele din fereastra de joc
int deplasareX1,deplasareX2,deplasareX3,deplasareY,latura;
int latimeFer,lungimeFer;
int meciuriP1,meciuriP2;
int panouVertical,distantaBorder=20;
int juc2Lin;
int juc2Col;
int counterImg=1;
int deplasareOriz,deplasareVer;
int laturaMenDim=120;
int updateDim;
int salvCol,salvLinie;
int miscJuc2=-1;
int limbaSist=1;
int piesaCurentaI,piesaCurentaJ;
mutari ai[100];
mutari juc2[100];
char*numePlayer1="P:1",*numePlayer2="P:2";
bool statusSunete;
bool AiStatus=false; //AI ON SAU OFF
bool avansat=false;//Random AI sau Advanced AI
bool functieVerificator=false;
bool memoreazaMiscare=false;
bool accesVerificator=false;
void click_handler(int x, int y);
void apelFunctieReset();
void apelFunctieUndo();
void deseneazaUI();
void deseneazaOptiuni();
void deseneazaMeniu();
void deseneazaHelp();
void deseneazaExtra();
void deseneazaOptiuniCulori();
void deseneazaVizualizator();
void deseneazaOptiuniDimensiuniTabla();
void deseneazaHighScore();
void seteazaDiametru();
void AiAdv();
void deseneazaTipJoc();
void startBgMusic();
void stopMusic();
void incarcaScor();
void deseneazaChenare();
void stopMusic();
void playClick();
void deseneazaDificultate();
void clickinDificultate();
void clickInExtra(int x,int y);
void clickInHIghScore(int x,int y);
void afiseazaMatr(int matr[][1000],int n,int m);
void initMatriceAI();
void AiPrimaMiscare();
void initControl()
{
    lungimeFer=1200;
    latimeFer=800;
    initwindow(lungimeFer,latimeFer);
    panouVertical=lungimeFer/3;
    numePlayer1="P:1";
    numePlayer2="P:2";
    statusSunete=false;
    int x,y;
    //EXPERIMENTAL
    /*
  panInfHor=latimeFer/20;
  panInfVert=lungimeFer/20;
  cout<<panInfHor;
  cout<<endl;
  cout<<panInfVert;
    settextstyle(EUROPEAN_FONT,HORIZ_DIR,panInfHor/2);
    outtextxy(latimeFer/2,panInfHor,"Player 1");
    settextstyle(EUROPEAN_FONT,HORIZ_DIR,panInfVert/2);
    outtextxy(panInfVert,100,"Player 2");
    */
    jucatorCurent=1;
}
void initializareTabla(){
    //Ionita Mihail Catalin && Georgica Marius
    int i,j;
    for(i=0;i<=n;i++)
        for(j=0;j<=n;j++)
            if(i%2==0 && j%2==1)
                a[i][j]=1;
            else if(i%2==1 && j%2==0)
                a[i][j]=2;
            else
                a[i][j]=3;
}
void incarcaScor()
{
        int x,y;
        while(gout>>x>>y)
    {
        meciuriP1=x;
        meciuriP2=y;
    }
    gout.close();
}
void deseneazaCerc(int x, int y, int c){
    //Ionita Mihail Catalin
    if(c==1){
        setcolor(culoareJuc1);
        setfillstyle(SOLID_FILL,culoareJuc1);
    }
    else    if(c==2){
        setcolor(culoareJuc2);
        setfillstyle(SOLID_FILL,culoareJuc2);
    }
    fillellipse(x,y,diametru/2,diametru/2);
}
void deseneazaConexiune(int i, int j, int mod, int c){
    //Georgica Marius
    if(mod==1){  //linie verticala
        if(c==1){
            setcolor(culoareJuc1);
            setfillstyle(SOLID_FILL,culoareJuc1);
        }
        else{
            setcolor(culoareJuc2);
            setfillstyle(SOLID_FILL,culoareJuc2);
        }
        bar(distantaBorder/2+j*diametru+diametru/3+3,distantaBorder/2+i*diametru-diametru/3,distantaBorder/2+(j+1)*diametru-diametru/3+3,distantaBorder/2+(i+1)*diametru+diametru/3);

    }
    else if(mod==2){  //linie orizontala
        if(c==1){
            setcolor(culoareJuc1);
            setfillstyle(SOLID_FILL,culoareJuc1);
        }
        else{
            setcolor(culoareJuc2);
            setfillstyle(SOLID_FILL,culoareJuc2);
        }
        bar(distantaBorder/2+j*diametru-diametru/3+3,distantaBorder/2+i*diametru+diametru/3,distantaBorder/2+(j+1)*diametru+diametru/3+3,distantaBorder/2+(i+1)*diametru-diametru/3);
    }
}
void afisareTabla(){
    int i,j;
    //Georgica Marius
    for(i=0;i<=n;i++)
        for(j=0;j<=n;j++)
            if(a[i][j]==1 || a[i][j]==2)
                //deseneazaCerc(vest+j*diametru+diametru/2,nord+i*diametru+diametru/2,a[i][j]);
                deseneazaCerc(distantaBorder/2+j*diametru+diametru/2+3,distantaBorder/2+i*diametru+diametru/2,a[i][j]);
            else if(a[i][j]==4){
                deseneazaConexiune(i,j,1,1);
            }
            else if(a[i][j]==5){
                deseneazaConexiune(i,j,2,1);
            }
            else if(a[i][j]==6){
                deseneazaConexiune(i,j,1,2);
            }
            else if(a[i][j]==7){
                deseneazaConexiune(i,j,2,2);
            }
}
void salveazaCopieMatrice(){
    //Georgica Marius
    int i,j;
    for(i=0;i<=n;i++)
        for(j=0;j<=n;j++)
            b[contorCopiiMatrici][i][j]=a[i][j];
}
void restaurareTabla(){
    //Georgica Marius
    int i,j;
    for(i=0;i<=n;i++)
        for(j=0;j<=n;j++)
            a[i][j]=b[contorCopiiMatrici][i][j];
}
void afisareJucatorCurent(){
    //Ionita Mihail Catalin
    settextstyle(3, HORIZ_DIR,18);
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    if(jucatorCurent==1){
        setcolor(culoareJuc1);
        outtextxy(getmaxx()/3*2+getmaxx()/3/2,getmaxy()/4*1/2,numePlayer1);
    }
    else{
        setcolor(culoareJuc2);
        outtextxy(getmaxx()/3*2+getmaxx()/3/2,getmaxy()/4*1/2,numePlayer2);
    }

}
void afisareCastigator(int c){
    //Ionita Mihail Catalin
    if(c==1){
        settextstyle(3,HORIZ_DIR,17);
        setcolor(culoareJuc1);
        switch(limbaSist){
        case 1:
            outtextxy(getmaxx()/3,getmaxy()/2," P:1 A CASTIGAT ! ");
            break;
        case 2:
            outtextxy(getmaxx()/3,getmaxy()/2," P:1 WON ! ");
            break;
        case 3:
            outtextxy(getmaxx()/3,getmaxy()/2," P:1 A GAGNÉ ! ");
            break;
        }
        PlaySoundA(TEXT("castigator.wav"),NULL,SND_ASYNC);
        castigator=0;
    }
    else if(c==2){
        setcolor(culoareJuc2);
        settextstyle(3,HORIZ_DIR,17);
        switch(limbaSist){
        case 1:
            outtextxy(getmaxx()/3,getmaxy()/2," P:2 A CASTIGAT ! ");
            break;
        case 2:
            outtextxy(getmaxx()/3,getmaxy()/2," P:2 WON ! ");
            break;
        case 3:
            outtextxy(getmaxx()/3,getmaxy()/2," P:2 A GAGNÉ ! ");
            break;
        }
        PlaySoundA(TEXT("castigator.wav"),NULL,SND_ASYNC);
        castigator=0;
    }
}
void solutieJucator1(int x, int y){
    //Georgica Marius
    if(a[x][y]==1 || a[x][y]==4 || a[x][y]==5){
        int copie=a[x][y];
        a[x][y]=9;
        if(x==n)
            castigator=1;
        else{
            solutieJucator1(x-1,y);
            solutieJucator1(x,y-1);
            solutieJucator1(x+1,y);
            solutieJucator1(x,y+1);
        }
        a[x][y]=copie; //la revenire din apel demarchez celula pentru a o putea accesa si in cadrul altei prelucrari

    }
}
void solutieJucator2(int x, int y){
    //Georgica Marius
    if(a[x][y]==2 || a[x][y]==6 || a[x][y]==7){
        int copie=a[x][y];
        a[x][y]=9;
        if(y==n)
            castigator=2;
        else{
            solutieJucator2(x-1,y);
            solutieJucator2(x,y-1);
            solutieJucator2(x+1,y);
            solutieJucator2(x,y+1);
        }
        a[x][y]=copie; //la revenire din apel demarchez celula pentru a o putea accesa si in cadrul altei prelucrari

    }
}
void verificaDacaACastigatCineva(){
    //Ionita Mihail Catalin
    int i,j;
    i=0;
    for(j=0;j<=n;j++)
        if(a[i][j]==1)
            solutieJucator1(i,j);
    j=0;
    for(i=0;i<=n;i++)
        if(a[i][j]==2)
            solutieJucator2(i,j);

}
void clickInMeniu(int x, int y){
    //Georgica Marius
    //playClick();
    if(x>getmaxx()/6*1 && x<getmaxx()/6*5 && y>getmaxy()/8*2 && y<(getmaxy()/8*4-getmaxy()/8/2) ){  //BUTON START
        deseneazaTipJoc();
        playClick();
        fereastraActiva=5;
    }
    if(x>getmaxx()/6*1 && x<getmaxx()/6*5 && y>getmaxy()/8*4 && y<(getmaxy()/8*6-getmaxy()/8/2) ){  //BUTON OPTIUNI
        deseneazaOptiuni();
        playClick();
        fereastraActiva=2;
    }
    if(x>getmaxx()/6 && x<getmaxx()/6*5 && y>getmaxy()/8*6 && y<(getmaxy()/8*8-getmaxy()/8/2) ){   //BUTON IESIRE
        closegraph();
        playClick();
    }
    if(x>=getmaxx()/16*15-50 && x<=getmaxx()-10 && y>=getmaxy()/16*14 && y<=getmaxy()-10)   //BUTON LIMBA
    {
        playClick();
        if(limbaSist==3)
            limbaSist=1;
        else limbaSist++;
        cleardevice();
        deseneazaMeniu();
    }
    if(x>=15 && x<=getmaxx()/8 && y<=getmaxy()-15 && y>=getmaxy()/8*7-15) //BUTON HELP
        {
            playClick();
            cleardevice();
            deseneazaHelp();
            fereastraActiva=7;
        }
}
void clickInJoc(int x, int y){
    //Ionita Mihail Catalin && Georgica Marius
    playClick();
    if(x>0 && y>5 && x<getmaxx()/3*2-5 && y<getmaxy()-5 && functieVerificator==false){ //click in spatiul de joc
        int linie=0,coloana=0;
        deseneazaChenare();
        x=x-distantaBorder/2;//corectie deplasare cursor
        y=y-distantaBorder/2;

        while(x>=diametru){
            x=x-diametru;
            coloana++;
        }
        while(y>=diametru){
            y=y-diametru;
            linie++;
        }
        if(a[linie][coloana]==3){
            if(jucatorCurent==1 && AiStatus==false){
                if((a[linie-1][coloana]==1) && (a[linie+1][coloana]==1 && AiStatus==false)){
                    deseneazaConexiune(linie,coloana,1,1);
                    contorCopiiMatrici++;
                    salveazaCopieMatrice();
                    a[linie][coloana]=4;
                    jucatorCurent=2;
                }
                else if(a[linie][coloana-1]==1 && a[linie][coloana+1]==1 && AiStatus==false){
                    deseneazaConexiune(linie,coloana,2,1);
                    contorCopiiMatrici++;
                    salveazaCopieMatrice();
                    a[linie][coloana]=5;
                    jucatorCurent=2;
                }

            }
            else if(jucatorCurent==2){
                if(a[linie-1][coloana]==2 && a[linie+1][coloana]==2){
                    deseneazaConexiune(linie,coloana,1,2);
                    contorCopiiMatrici++;
                    salveazaCopieMatrice();
                    a[linie][coloana]=6;
                    jucatorCurent=1;
                    miscJuc2++;
                 juc2[miscJuc2].linie=linie+1;
                 juc2[miscJuc2].coloana=coloana;

                }
                else if(a[linie][coloana-1]==2 && a[linie][coloana+1]==2){
                    deseneazaConexiune(linie,coloana,2,2);
                    contorCopiiMatrici++;
                    salveazaCopieMatrice();
                    a[linie][coloana]=7;
                    jucatorCurent=1;
                    miscJuc2++;
                    juc2[miscJuc2].linie=linie;
                    juc2[miscJuc2].coloana=coloana+1;
                }
                if(AiStatus==true)
                {
                    if(avansat==false)
                    {
                        //AI modul standard
                        //reinitializam seed-ul functiei rand;
                        srand((unsigned)time(&tmp));
                        int randLin,randCol;
                        //range 1,n;
                        randLin=rand()%9;
                        randCol=rand()%9;
                    while(jucatorCurent==1)
                    {
                        delay(2);
                if(a[randLin-1][randCol]==1 && a[randLin+1][randCol]==1 && a[randLin][randCol]==3){
                    deseneazaConexiune(randLin,randCol,1,1);
                    contorCopiiMatrici++;
                    salveazaCopieMatrice();
                    a[randLin][randCol]=4;
                    jucatorCurent=2;
                }
                else if(a[randLin][randCol-1]==1 && a[randLin][randCol+1]==1 && a[randLin][randCol]==3){
                    deseneazaConexiune(randLin,randCol,2,1);
                    contorCopiiMatrici++;
                    salveazaCopieMatrice();
                    a[randLin][randCol]=5;
                    jucatorCurent=2;
                }
                  randLin=rand()%9;
                  randCol=rand()%9;
                    }
                    }
                    else
                    {

                        if(juc2[miscJuc2].coloana+2==n && miscJuc2>=n/2-1 && a[juc2[miscJuc2].linie][juc2[miscJuc2].coloana+1]==3)
                        {
                            int aux1=juc2[miscJuc2].linie;
                            int aux2=juc2[miscJuc2].coloana+1;
                            deseneazaConexiune(aux1,aux2,1,1);
                            a[aux1][aux2]=4;
                            jucatorCurent=2;
                            piesaCurentaI=juc2[miscJuc2].linie-1;
                            piesaCurentaJ=juc2[miscJuc2].coloana+2;
                            counterMisc++;
                            ai[counterMisc].linie=piesaCurentaI;
                            ai[counterMisc].coloana=piesaCurentaJ;
                            counterMisc++;
                            ai[counterMisc].linie=juc2[miscJuc2].linie+1;
                            ai[counterMisc].coloana=juc2[miscJuc2].coloana+1;
                            memoreazaMiscare=true;
                           // salvCol=juc2[miscJuc2].coloana+1;
                           // salvLinie=juc2[miscJuc2].linie+1;
                          // ai[counterMisc+4].linie=juc2[miscJuc2].linie;
                          // ai[counterMisc+4].coloana=juc2[miscJuc2].coloana+2;
                        }
                      //  if(juc2[miscJuc2-1].coloana-2==0 && miscJuc2>=n/2-1)
                        if(a[piesaCurentaI+1][piesaCurentaJ]==3 && jucatorCurent==1)
                        {
                                a[piesaCurentaI+1][piesaCurentaJ]=4;
                                deseneazaConexiune(piesaCurentaI+1,piesaCurentaJ,1,1);
                                piesaCurentaI=piesaCurentaI+2;
                                //contorCopiiMatrici++;
                                //salveazaCopieMatrice();
                                jucatorCurent=2;
                                counterMisc=counterMisc+1;
                                ai[counterMisc].linie=piesaCurentaI;
                                ai[counterMisc].coloana=piesaCurentaJ;
                        }
                        else if(a[piesaCurentaI][piesaCurentaJ+1]==3 && piesaCurentaJ+1<n && jucatorCurent==1 )
                        {
                                a[piesaCurentaI][piesaCurentaJ+1]=5;
                                deseneazaConexiune(piesaCurentaI,piesaCurentaJ+1,2,1);
                                piesaCurentaJ=piesaCurentaJ+2;
                               // contorCopiiMatrici++;
                                //salveazaCopieMatrice();
                                jucatorCurent=2;
                               counterMisc=counterMisc+1;
                                ai[counterMisc].linie=piesaCurentaI;
                                ai[counterMisc].coloana=piesaCurentaJ;

                        }
                        else if(a[piesaCurentaI][piesaCurentaJ-1]==3 && piesaCurentaJ-1>0 && jucatorCurent==1)
                        {
                                 a[piesaCurentaI][piesaCurentaJ-1]=5;
                                deseneazaConexiune(piesaCurentaI,piesaCurentaJ-1,2,1);
                                piesaCurentaJ=piesaCurentaJ-2;
                                //contorCopiiMatrici++;
                                jucatorCurent=2;
                                //salveazaCopieMatrice();
                                counterMisc=counterMisc+1;
                                ai[counterMisc].linie=piesaCurentaI;
                                ai[counterMisc].coloana=piesaCurentaJ;

                        }
                        else
                        {
                            int miscareGasita=false;
                            if(jucatorCurent==1)
                            {
                                for(int i=counterMisc; i>=0 && miscareGasita!=true ;i--)
                                {

                                    if(a[ai[i].linie+1][ai[i].coloana]==3)
                                        {
                                            miscareGasita=true;
                                            a[ai[i].linie+1][ai[i].coloana]=4;
                                            deseneazaConexiune(ai[i].linie+1,ai[i].coloana,1,1);
                                            piesaCurentaI=ai[i].linie+2;
                                            piesaCurentaJ=ai[i].coloana;
                                            jucatorCurent=2;
                                            counterMisc++;
                                            ai[counterMisc].linie=piesaCurentaI;
                                            ai[counterMisc].coloana=piesaCurentaJ;
                                        }
                                     if(a[ai[i].linie][ai[i].coloana+1]==3 && ai[i].coloana+1<n && miscareGasita!=true)
                                    {
                                            miscareGasita=true;
                                            a[ai[i].linie][ai[i].coloana+1]=5;
                                            deseneazaConexiune(ai[i].linie,ai[i].coloana+1,2,1);
                                            piesaCurentaI=ai[i].linie;
                                            piesaCurentaJ=ai[i].coloana+2;
                                            jucatorCurent=2;
                                            counterMisc++;
                                            ai[counterMisc].linie=piesaCurentaI;
                                            ai[counterMisc].coloana=piesaCurentaJ;
                                }
                                    if(a[ai[i].linie][ai[i].coloana-1]==3 && ai[i].coloana-1>0 && miscareGasita!=true)
                                    {
                                            miscareGasita=true;
                                            a[ai[i].linie][ai[i].coloana-1]=5;
                                            deseneazaConexiune(ai[i].linie,ai[i].coloana-1,2,1);
                                            piesaCurentaI=ai[i].linie;
                                            piesaCurentaJ=ai[i].coloana-2;
                                            jucatorCurent=2;
                                            counterMisc++;
                                            ai[counterMisc].linie=piesaCurentaI;
                                            ai[counterMisc].coloana=piesaCurentaJ;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        verificaDacaACastigatCineva();
        if(castigator==1)
        {
            accesVerificator=true;
            afisareCastigator(1);
              meciuriP1++;
                gout<<meciuriP1<<" "<<meciuriP2;
                gout<<endl;
                salveazaCopieMatrice();
        }
        else if(castigator==2)
        {
            accesVerificator=true;
            afisareCastigator(2);
                meciuriP2++;
                salveazaCopieMatrice();
        }
        afisareJucatorCurent();
    }
    if(x>getmaxx()/3*2+5 && y>getmaxy()/4*1+5 && x<getmaxx()-5 && y<getmaxy()/4*2-2 && AiStatus==false)//BUTON UNDO
        apelFunctieUndo();
    if(x>getmaxx()/3*2+5 && y>getmaxy()/4*2+5 && x<getmaxx()-5 && y<getmaxy()/4*3-2) //BUTON RESET
        apelFunctieReset();
    if(x>getmaxx()/3*2+5 && y>getmaxy()/4*3+5 && x<getmaxx()-5 && y<getmaxy()/4*4-2){//BUTON BACK
            closegraph();     //am preferat sa inchid si redeschid fereastra deoarece erau probleme legate de culoarea de fundal a meniului si a textului
            initwindow(lungimeFer,latimeFer);   //probleme=fundal alb cu text alb, nu am reusit sa repar fara sa stric celelalte meniuri
            registermousehandler(WM_LBUTTONDOWN, click_handler);
            deseneazaMeniu();
            AiStatus=false;
            avansat=false;
            fereastraActiva=0;
            if(functieVerificator)
            functieVerificator=false;
            startBgMusic();
        }
}
void clickInMeniuOptiuni(int x, int y){
    //Georgica Marius && Ionita Mihail Catalin
    //playClick();
    if(x>getmaxx()/6*1 && y>getmaxy()/8*1 && x<getmaxx()/6*5 && y<getmaxy()/8*3-getmaxy()/8/2){ //OPTIUNI CULORI
        cleardevice();
        playClick();
        deseneazaOptiuniCulori();
        fereastraActiva=3;
    }
    if(x>getmaxx()/6*1 && y>getmaxy()/8*3 && x<getmaxx()/6*5 && y<getmaxy()/8*5-getmaxy()/8/2){ //OPTIUNI DIMENSIUNI TABLA
        playClick();
        cleardevice();
        deseneazaOptiuniDimensiuniTabla();
        fereastraActiva=4;
    }
    if(x>10 && y>getmaxy()/8*7 && x<getmaxx()/6*2 && y<getmaxy()-10){ //BACK
        cleardevice();
        playClick();
        deseneazaMeniu();
        fereastraActiva=0;
    }
    if(x>getmaxx()/6*1 && y>getmaxy()/8*5 && x<getmaxx()/6*5 && y<getmaxy()/8*7-getmaxy()/8/2)
    {//Sunete ON SAU OFF
        if(statusSunete==true)
        {
         statusSunete=false;
         cout<<limbaSist<<" ";
         switch(limbaSist)
         {
         case 1:
            {
                cleardevice();
                deseneazaOptiuni();
                outtextxy(getmaxx()/2,getmaxy()/8*7-getmaxy()/8,"SUNETE:OFF ");
                break;
            }
         case 2:
            {
                cleardevice();
                deseneazaOptiuni();
                outtextxy(getmaxx()/2,getmaxy()/8*7-getmaxy()/8,"SOUNDS:OFF ");
                break;
            }

         case 3:
            {
                cleardevice();
                deseneazaOptiuni();
                outtextxy(getmaxx()/2,getmaxy()/8*7-getmaxy()/8,"SONS:OFF ");
                break;
            }
         }
         stopMusic();
        }
        else
        {
            statusSunete=true;
            cout<<limbaSist<<" ";
            switch(limbaSist)
            {
             case 1:
                {
                    cleardevice();
                    deseneazaOptiuni();
                    outtextxy(getmaxx()/2,getmaxy()/8*7-getmaxy()/8,"SUNETE:ON ");
                    break;
                }
             case 2:
                 {
                    cleardevice();
                    deseneazaOptiuni();
                    outtextxy(getmaxx()/2,getmaxy()/8*7-getmaxy()/8,"SOUNDS:ON ");
                    break;
                }
             case 3:
                {
                   cleardevice();
                    deseneazaOptiuni();
                    outtextxy(getmaxx()/2,getmaxy()/8*7-getmaxy()/8,"SONS:ON ");
                    break;
                }
             }
            startBgMusic();
        }
    }
}
void clickInHelp(int x,int y)
{
    //Georgica Marius
    //playClick();
    if(x>10 && y>getmaxy()/8*7 && x<getmaxx()/6*2 && y<getmaxy()-10){ //BACK
        cleardevice();
        playClick();
        deseneazaMeniu();
        fereastraActiva=0;
    }
    // rectangle(getmaxx()-20,getmaxy()/8*7,getmaxx()/16*12,getmaxy()-20);
    if(x<=getmaxx()-20 && x>=getmaxx()/16*12 && y>=getmaxy()/8*7 && y<=getmaxy()-20)
    {
        playClick();
        cout<<1;
        counterImg++;
        if(counterImg==3)
            counterImg=1;
        if(counterImg==1)
        {
            cleardevice();
            deseneazaHelp();
            readimagefile("tutorial.bmp",getmaxx()/16*3,20,getmaxx()/16*12,getmaxy()-150);
        }
    else {
            cleardevice();
            deseneazaHelp();
            readimagefile("conexiuni.bmp",getmaxx()/16*3,20,getmaxx()/16*12,getmaxy()-150);
    }
    }
}
void clickInOptiuniCulori(int x, int y){
    //Georgica Marius
    playClick();
    int linie=0;
    int coloana=0;

    if(x>getmaxx()/6*1-2*latura && y>deplasareY && x<getmaxx()/6*1+2*latura && y<deplasareY+4*latura){  //CULORI PLAYER1
        x=x-deplasareX1;
        y=y-deplasareY;
        while(x>=latura){
            x=x-latura;
            coloana++;
        }
        while(y>=latura){
            y=y-latura;
            linie++;
        }
        culoareJuc1=linie*4+coloana;
    }

    if(x>getmaxx()/6*3-2*latura && y>deplasareY && x<getmaxx()/6*3+2*latura && y<deplasareY+4*latura){ //CULORI PLAYER2
        x=x-deplasareX2;
        y=y-deplasareY;
        while(x>=latura){
            x=x-latura;
            coloana++;
        }
        while(y>=latura){
            y=y-latura;
            linie++;
        }
        culoareJuc2=linie*4+coloana;
    }

    if(x>getmaxx()/6*5-2*latura && y>deplasareY && x<getmaxx()/6*5+2*latura && y<deplasareY+4*latura){ //CULORI TABLA
        x=x-deplasareX3;
        y=y-deplasareY;
        while(x>=latura){
            x=x-latura;
            coloana++;
        }
        while(y>=latura){
            y=y-latura;
            linie++;
        }
        culoareTabla=linie*4+coloana;
        if(culoareTabla==0)
            culoareInterfata=15;
    }
    cleardevice();
    deseneazaOptiuniCulori();

    if(x>=10 && y>=getmaxy()/8*7 && x<=getmaxx()/6*2 && y<=getmaxy()-10){ //BACK
        cleardevice();
        deseneazaOptiuni();
        fereastraActiva=2;
    }
}
void clickInHighScore(int x,int y)
{
    //Ionita Mihail Catalin
    playClick();
    if(x<=getmaxx()-20 && x>=getmaxx()/16*12 && y>=getmaxy()/8*7 && y<=getmaxy()-20)
    {
        gout<<0<<" "<<0;
        deseneazaHighScore();
    }
}
void clickInOptiuniDimensiuni(int x, int y){
    playClick();
    //  Ionita Mihail Catalin
    int startAbs=getmaxx()/2-(laturaMenDim+laturaMenDim/2);
    int compus;
    int startOrd=getmaxy()/16*3+30;
    if(x>=10 && y>=getmaxy()/8*7 && x<=getmaxx()/6*2 && y<=getmaxy()-10){ //BACK
        cleardevice();
        deseneazaOptiuni();
        fereastraActiva=2;
        updateDim=0;
    }
    if(x>=startAbs&& y>=startOrd && x<=startAbs+3*laturaMenDim && y<=startOrd+4*laturaMenDim)
    {

        int linie=0,coloana=0;
        x=x-startAbs;
        y=y-startOrd;
        while(x>=laturaMenDim)
        {
            x=x-laturaMenDim;
            coloana++;
        }
        while(y>=laturaMenDim)
        {
            y=y-laturaMenDim;
            linie++;
        }
        compus=3*linie+coloana+1;
        if(compus>=1  && compus<=9)
        {
            updateDim=updateDim*10+compus;
        }
        if(compus==10)
            n=updateDim;
        if(compus==11)
            updateDim=updateDim*10;
        if(compus==12)
            updateDim=0;
    }
    seteazaDiametru();
}
void clickInExtra(int x,int y)
{
    //Ionita Mihail Catalin
    playClick();
    settextstyle(3,HORIZ_DIR,19);
    if(x>getmaxx()/6*1 && y>getmaxy()/8*1 && x<getmaxx()/6*5 && y<getmaxy()/8*3-getmaxy()/8/2){
        fereastraActiva=10;
        cleardevice();
        deseneazaHighScore();
    }
    if(x>getmaxx()/6*1 && y>getmaxy()/8*3 && x<getmaxx()/6*5 && y<getmaxy()/8*5-getmaxy()/8/2 ){
            if(accesVerificator==true)
            {
            cleardevice();
            fereastraActiva=9;
            functieVerificator=true;
            deseneazaChenare();
            deseneazaUI();
            restaurareTabla();
            afisareTabla();
            }
            else
            {
                settextstyle(3,HORIZ_DIR,12);
                outtextxy(getmaxx()/12*7,getmaxy()-30,"TREBUIE SA JUCATI MINIM UN MECI");
            }
    }
     if(x>10 && y>getmaxy()/8*7 && x<getmaxx()/6*2 && y<getmaxy()-10){
        cleardevice();
        deseneazaTipJoc();
        fereastraActiva=5;
    }
}
void clickinTipJoc(int x,int y)
{
    //Georgica Marius
    playClick();
    if(x>getmaxx()/6*1 && y>getmaxy()/8*1 && x<getmaxx()/6*5 && y<getmaxy()/8*3-getmaxy()/8/2){
            fereastraActiva=1;
            deseneazaUI();
           // initializareTabla();
    }
    if(x>getmaxx()/6*1 && y>getmaxy()/8*3 && x<getmaxx()/6*5 && y<getmaxy()/8*5-getmaxy()/8/2){
        cleardevice();
        deseneazaDificultate();
        fereastraActiva=6;
    }
    if(x>getmaxx()/6*1 && y>getmaxy()/8*5 && x<getmaxx()/6*5 && y<getmaxy()/8*7-getmaxy()/8/2){
        cleardevice();
        deseneazaExtra();
        fereastraActiva=8;
    }
    if(x>10 && y>getmaxy()/8*7 && x<getmaxx()/6*2 && y<getmaxy()-10){
        cleardevice();
        deseneazaMeniu();
        fereastraActiva=0;
    }
}
void clickinDificultate(int x,int y)
{
    //Georgica Marius && Ionita Mihail Catalin
    //playClick();
      if(x>getmaxx()/6*1 && y>getmaxy()/8*1 && x<getmaxx()/6*5 && y<getmaxy()/8*3-getmaxy()/8/2){ // RANDOM AI
        fereastraActiva=1;
        deseneazaUI();
        playClick();
        AiStatus=true;
        AiPrimaMiscare();
    }
    if(x>getmaxx()/6*1 && y>getmaxy()/8*3 && x<getmaxx()/6*5 && y<getmaxy()/8*5-getmaxy()/8/2){ // ADVANCED AI
        fereastraActiva=1;
        AiStatus=true;
        playClick();
        avansat=true;
        deseneazaUI();
        AiAdv();
    }
    if(x>10 && y>getmaxy()/8*7 && x<getmaxx()/6*2 && y<getmaxy()-10){ //INAPOI
        cleardevice();
        playClick();
        deseneazaTipJoc();
        fereastraActiva=5;
    }
}
void click_handler(int x, int y){ //transmite coordonatele click-ului(x si y) functiilor corespunzatoare ferestrei curente
    //Georgica Marius && Ionita Mihail Catalin
        switch(fereastraActiva)
    {
        case 0:{
            clickInMeniu(x,y);
            break;
        }
        case 1:{
            clickInJoc(x,y);
            break;
        }
        case 2:{
            clickInMeniuOptiuni(x,y);
            break;
        }
        case 3:{
            clickInOptiuniCulori(x,y);
            break;
            }
        case 4:{
            clickInOptiuniDimensiuni(x,y);
            break;
            }
        case 5:{
            clickinTipJoc(x,y);
            break;
        }
        case 6:{
            clickinDificultate(x,y);
            break;
        }
        case 7:
            {
                clickInHelp(x,y);
                break;
            }
        case 8:
            {
                clickInExtra(x,y);
                break;
            }
        case 9:
            {
                clickInJoc(x,y);
                break;
            }
        case 10:
            {
                clickInHIghScore(x,y);
                break;
            }
        }
    }
void deseneazaExtra()
{
    //Georgica Marius
    setcolor(INT_COLOR);
    settextstyle(3,HORIZ_DIR,18);
    rectangle(getmaxx()/6*1,getmaxy()/8*1,getmaxx()/6*5,getmaxy()/8*3-getmaxy()/8/2);
    rectangle(getmaxx()/6*1,getmaxy()/8*3,getmaxx()/6*5,getmaxy()/8*5-getmaxy()/8/2);
    rectangle(10,getmaxy()/8*7,getmaxx()/6*2,getmaxy()-10);
    switch(limbaSist)
    {
        case 1:
            {
                setcolor(WHITE);
                 outtextxy(getmaxx()/2,getmaxy()/8*3-getmaxy()/8,"SCORUL CEL MAI MARE");
                 outtextxy(getmaxx()/2,getmaxy()/8*5-getmaxy()/8,"VIZUALIZATOR MISCARI");
                 outtextxy(getmaxx()/6,(getmaxy())-getmaxy()/8/3,"INAPOI");
                break;
            }
        case 2:
            {
                setcolor(WHITE);
                outtextxy(getmaxx()/2,getmaxy()/8*3-getmaxy()/8,"HIGHSCORE");
                 outtextxy(getmaxx()/2,getmaxy()/8*5-getmaxy()/8,"MOVES VISUALiZATION");
                 outtextxy(getmaxx()/6,(getmaxy())-getmaxy()/8/3,"BACK");
                break;
            }
        case 3:
            {
                setcolor(WHITE);
                outtextxy(getmaxx()/2,getmaxy()/8*3-getmaxy()/8,"SCORE ÉLEVÉ");
                 outtextxy(getmaxx()/2,getmaxy()/8*5-getmaxy()/8,"VISUALISATEUR DE MOUVEMENTS");
                 outtextxy(getmaxx()/6,(getmaxy())-getmaxy()/8/3,"RETOUR");
                break;
            }
    }
}
void deseneazaHelp()
{
    //Georgica Marius
            setcolor(INT_COLOR);
            rectangle(10,getmaxy()/8*7,getmaxx()/6*2,getmaxy()-10);
            rectangle(getmaxx()-20,getmaxy()/8*7,getmaxx()/16*12,getmaxy()-20);
            readimagefile("tutorial.bmp",getmaxx()/16*3,20,getmaxx()/16*12,getmaxy()-150);
            settextstyle(3,HORIZ_DIR,15);
     switch(limbaSist)
    {
        case 1:
            {
                setcolor(WHITE);
                outtextxy(getmaxx()/6,(getmaxy())-getmaxy()/8/2,"INAPOI");
                outtextxy(getmaxx()-160,getmaxy()-60,"URMATORUL");
            break;
            }
        case 2:
            {
                setcolor(WHITE);
                outtextxy(getmaxx()/6,(getmaxy())-getmaxy()/8/2,"BACK");
                outtextxy(getmaxx()-160,getmaxy()-60,"NEXT");
            break;
            }
        case 3:
            {
                setcolor(WHITE);
                outtextxy(getmaxx()/6,(getmaxy())-getmaxy()/8/2,"RETOUR");
                outtextxy(getmaxx()-160,getmaxy()-60,"SUIVANT");
            break;
            }
    }
}
void deseneazaChenare(){
    //Ionita Mihail Catalin
    setcolor(culoareInterfata); //culoarea chenarelor
    setlinestyle(SOLID_LINE,1,3);
    rectangle(5,5,getmaxx()/3*2,getmaxy()-5);  //tabla
    rectangle(getmaxx()/3*2+5,5,getmaxx()-5,getmaxy()/4*1);  //chenar jucator curent
    rectangle(getmaxx()/3*2+5,getmaxy()/4*1+5,getmaxx()-5,getmaxy()/4*2);//chenar casuta libera? ce punem aici
    rectangle(getmaxx()/3*2+5,getmaxy()/4*2+5,getmaxx()-5,getmaxy()/4*3);  //chenar undo
    rectangle(getmaxx()/3*2+5,getmaxy()/4*3+5,getmaxx()-5,getmaxy()/4*4-2); //chenar reset
    settextstyle(3,HORIZ_DIR,17);

    switch(limbaSist)
    {
    case 1:
        {
            if(AiStatus==false)
            outtextxy(getmaxx()/3*2+getmaxx()/3/2,getmaxy()/4*1+getmaxy()/4/2,"ANULEAZA");
            if(functieVerificator==false)
            outtextxy(getmaxx()/3*2+getmaxx()/3/2,getmaxy()/4*2+getmaxy()/4/2,"RESETEAZA");
            outtextxy(getmaxx()/3*2+getmaxx()/3/2,getmaxy()/4*3+getmaxy()/4/2,"INAPOI");
            break;
        }
    case 2:
        {
            if(AiStatus==false)
            outtextxy(getmaxx()/3*2+getmaxx()/3/2,getmaxy()/4*1+getmaxy()/4/2,"UNDO");
            if(functieVerificator==false)
            outtextxy(getmaxx()/3*2+getmaxx()/3/2,getmaxy()/4*2+getmaxy()/4/2,"RESET");
            outtextxy(getmaxx()/3*2+getmaxx()/3/2,getmaxy()/4*3+getmaxy()/4/2,"BACK");
            break;
        }
    case 3:
        {
            if(AiStatus==false)
            outtextxy(getmaxx()/3*2+getmaxx()/3/2,getmaxy()/4*1+getmaxy()/4/2,"ANNULER");
            if(functieVerificator==false)
            outtextxy(getmaxx()/3*2+getmaxx()/3/2,getmaxy()/4*2+getmaxy()/4/2,"RÉINITIALISER");
            outtextxy(getmaxx()/3*2+getmaxx()/3/2,getmaxy()/4*3+getmaxy()/4/2,"RETOUR");
            break;
        }
    }

}
void apelFunctieReset(){
    //Georgica Marius
    initializareTabla();
    jucatorCurent=1;
    castigator=0;
    contorCopiiMatrici=-1;
    deseneazaUI();
    if(AiStatus==true && avansat==true)
        AiAdv();
    if(AiStatus==true && avansat==false)
        AiPrimaMiscare();
    if(AiStatus==true && avansat==true)
    {
        for(int i=0;i<100;i++)
        {
            miscJuc2=0;
            counterMisc=0;
            juc2[i].linie=0;
             juc2[i].coloana=0;
            ai[i].linie=0;
            ai[i].coloana=0;
        }
    }
}
void apelFunctieUndo(){
    //Georgica Marius
    if(contorCopiiMatrici>=0){
        if(jucatorCurent==1)
            jucatorCurent=2;
        else
            jucatorCurent=1;
        castigator=0; //resetare castigator pentru reverificare
        cleardevice();
        restaurareTabla();
        afisareTabla();
        afisareJucatorCurent();
        deseneazaChenare();
        contorCopiiMatrici--;
    }
}
void deseneazaUI()
{
    //Ionita Mihail Catalin
    cleardevice();
    registermousehandler(WM_LBUTTONDOWN, click_handler);
    setfillstyle(SOLID_FILL,culoareTabla);
    setbkcolor(culoareTabla);
    fillellipse(getmaxx()/2,getmaxy()/2,1000,1000);
    //restaurareTabla();
    initializareTabla();
    afisareTabla();
    afisareJucatorCurent();
    deseneazaChenare();
}

void deseneazaMeniu(){
    //Georgica Marius && Ionita Mihail Catalin
    registermousehandler(WM_LBUTTONDOWN, click_handler);
    setcolor(INT_COLOR);
    readimagefile("title.bmp",getmaxx()/12*3,0,getmaxx()/12*9,getmaxy()/16*5);
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    settextstyle(3,HORIZ_DIR,19);
    rectangle(getmaxx()/6*1,getmaxy()/8*2,getmaxx()/6*5,getmaxy()/8*4-getmaxy()/8/2); //INCEPE JOC
    rectangle(getmaxx()/6*1,getmaxy()/8*4,getmaxx()/6*5,getmaxy()/8*6-getmaxy()/8/2); //OPTIUNI
    rectangle(getmaxx()/6*1,getmaxy()/8*6,getmaxx()/6*5,getmaxy()/8*8-getmaxy()/8/2); //IESIRE
    rectangle(getmaxx()/16*15-50,getmaxy()/16*14,getmaxx()-10,getmaxy()-10); //BUTON LIMBA
    rectangle(getmaxx()/6-15,getmaxy()/8*2-15,getmaxx()/6*5+15,getmaxy()/8*8-getmaxy()/8/2+15); //chenar mare peste start,optiuni,iesire
    rectangle(15,getmaxy()/8*7-15,getmaxx()/8,getmaxy()-15); //BUTON HELP
    setcolor(WHITE);
    outtextxy(getmaxx()/26+40,getmaxy()/20*19+12,"?");
    switch(limbaSist)
    {
    case 1:
        {
        setcolor(WHITE);
        outtextxy(getmaxx()/20*19+10,getmaxy()/20*19+17,"RO");
        break;
        }
    case 2:
        {
            setcolor(WHITE);
            outtextxy(getmaxx()/20*19+10,getmaxy()/20*19+17,"EN");
            break;
        }
    case 3:
        {
            setcolor(WHITE);
            outtextxy(getmaxx()/20*19+10,getmaxy()/20*19+17,"FR");
            break;
        }
    }
    switch(limbaSist)
    {
        case 1:
            {
                setcolor(WHITE);
            outtextxy(getmaxx()/2,getmaxy()/8*4-getmaxy()/8,"INCEPE JOC");
            outtextxy(getmaxx()/2,getmaxy()/8*6-getmaxy()/8,"OPTIUNI");
            outtextxy(getmaxx()/2,getmaxy()/8*8-getmaxy()/8,"IESIRE");
            break;
            }
        case 2:
            {
                setcolor(WHITE);
            outtextxy(getmaxx()/2,getmaxy()/8*4-getmaxy()/8,"START GAME");
            outtextxy(getmaxx()/2,getmaxy()/8*6-getmaxy()/8,"OPTIONS");
            outtextxy(getmaxx()/2,getmaxy()/8*8-getmaxy()/8,"EXIT");
            break;
            }
        case 3:
            {
                setcolor(WHITE);
            outtextxy(getmaxx()/2,getmaxy()/8*4-getmaxy()/8,"DÉMARRER JEU");
            outtextxy(getmaxx()/2,getmaxy()/8*6-getmaxy()/8,"REGLAGES");
            outtextxy(getmaxx()/2,getmaxy()/8*8-getmaxy()/8,"SORTIE");
            break;
            }
    }

}
void deseneazaOptiuni(){

    //Georgica Marius
    cleardevice();
    setcolor(INT_COLOR);
    settextstyle(3,HORIZ_DIR,18);
    rectangle(getmaxx()/6*1,getmaxy()/8*1,getmaxx()/6*5,getmaxy()/8*3-getmaxy()/8/2);
    rectangle(getmaxx()/6*1,getmaxy()/8*3,getmaxx()/6*5,getmaxy()/8*5-getmaxy()/8/2);
    rectangle(getmaxx()/6*1,getmaxy()/8*5,getmaxx()/6*5,getmaxy()/8*7-getmaxy()/8/2);
    rectangle(10,getmaxy()/8*7 ,getmaxx()/6*2,getmaxy()-10);
    switch(limbaSist)
    {
        case 1:
            {
            setcolor(WHITE);
            outtextxy(getmaxx()/2,getmaxy()/8*3-getmaxy()/8,"CULORI");
            outtextxy(getmaxx()/2,getmaxy()/8*5-getmaxy()/8,"DIMENSIUNE TABLA");
            outtextxy(getmaxx()/6,(getmaxy())-getmaxy()/8/3,"INAPOI");
            if(statusSunete==true)
                outtextxy(getmaxx()/2,getmaxy()/8*7-getmaxy()/8,"SUNETE:ON ");
            else
                outtextxy(getmaxx()/2,getmaxy()/8*7-getmaxy()/8,"SUNETE:OFF ");
            break;
            }
        case 2:
            setcolor(WHITE);
            outtextxy(getmaxx()/2,getmaxy()/8*3-getmaxy()/8,"COLORS");
            outtextxy(getmaxx()/2,getmaxy()/8*5-getmaxy()/8,"GAME TABLE SIZE");
            outtextxy(getmaxx()/6,(getmaxy())-getmaxy()/8/3,"BACK");
            if(statusSunete==true)
                outtextxy(getmaxx()/2,getmaxy()/8*7-getmaxy()/8,"SOUNDS:ON ");
            else
                outtextxy(getmaxx()/2,getmaxy()/8*7-getmaxy()/8,"SOUNDS:OFF ");
            {
            break;
            }
        case 3:
            {
            setcolor(WHITE);
            outtextxy(getmaxx()/2,getmaxy()/8*3-getmaxy()/8,"COULEURS");
            outtextxy(getmaxx()/2,getmaxy()/8*5-getmaxy()/8,"DIMENSION DU JEU");
            outtextxy(getmaxx()/6,(getmaxy())-getmaxy()/8/3,"RETOUR");
            if(statusSunete==true)
                outtextxy(getmaxx()/2,getmaxy()/8*7-getmaxy()/8,"SONS:ON ");
            else
                outtextxy(getmaxx()/2,getmaxy()/8*7-getmaxy()/8,"SONS:OFF ");
            break;
            }
    }
}
void deseneazaOptiuniCulori(){
    //Georgica Marius
    setcolor(INT_COLOR);
    latura=60;
    int culoare=0;
    deplasareX1=getmaxx()/6*1-2*latura;
    deplasareX2=getmaxx()/6*3-2*latura;
    deplasareX3=getmaxx()/6*5-2*latura;
    deplasareY=getmaxy()/4*2;
    settextstyle(3,HORIZ_DIR,16);
    for(int j=0;j<=3;j++)            //am inversat i cu j deoarece matricea era desenata de sus in jos pe linii si apoi coloane
        for(int i=0;i<=3;i++){
            setfillstyle(SOLID_FILL,culoare);
            bar(deplasareX1+i*latura,deplasareY+j*latura,deplasareX1+(i+1)*latura,deplasareY+(j+1)*latura);
            bar(deplasareX2+i*latura,deplasareY+j*latura,deplasareX2+(i+1)*latura,deplasareY+(j+1)*latura);
            bar(deplasareX3+i*latura,deplasareY+j*latura,deplasareX3+(i+1)*latura,deplasareY+(j+1)*latura);
            culoare++;
        }

    settextstyle(3,HORIZ_DIR,18);
    rectangle(10,getmaxy()/8*7 ,getmaxx()/6*2,getmaxy()-10);//chenar BACK
    //patratelele care arata culoarea curenta
    if(culoareJuc1==BLACK){
        setcolor(WHITE);
        rectangle(getmaxx()/6*1-latura/2-1,getmaxy()/9*3-latura/2-1,getmaxx()/6*1+latura/2+1,getmaxy()/9*3+latura/2+1);
    }
    setfillstyle(SOLID_FILL,culoareJuc1);
    bar(getmaxx()/6*1-latura/2,getmaxy()/9*3-latura/2,getmaxx()/6*1+latura/2,getmaxy()/9*3+latura/2);

    if(culoareJuc2==BLACK){
        setcolor(WHITE);
        rectangle(getmaxx()/6*3-latura/2-1,getmaxy()/9*3-latura/2-1,getmaxx()/6*3+latura/2+1,getmaxy()/9*3+latura/2+1);
    }
    setfillstyle(SOLID_FILL,culoareJuc2);
    bar(getmaxx()/6*3-latura/2,getmaxy()/9*3-latura/2,getmaxx()/6*3+latura/2,getmaxy()/9*3+latura/2);

    if(culoareTabla==BLACK){
        setcolor(WHITE);
        rectangle(getmaxx()/6*5-latura/2-1,getmaxy()/9*3-latura/2-1,getmaxx()/6*5+latura/2+1,getmaxy()/9*3+latura/2+1);
    }
    setfillstyle(SOLID_FILL,culoareTabla);
    bar(getmaxx()/6*5-latura/2,getmaxy()/9*3-latura/2,getmaxx()/6*5+latura/2,getmaxy()/9*3+latura/2);

        switch(limbaSist)
    {
        case 1:
            {
                setcolor(WHITE);
                 settextstyle(3,HORIZ_DIR,17);
            outtextxy(getmaxx()/2,50,"ALEGETI CULORILE DORITE:");
            outtextxy(getmaxx()/6*1,getmaxy()/4*1,"JUCATOR 1");
            outtextxy(getmaxx()/6*3,getmaxy()/4*1,"JUCATOR 2");
            outtextxy(getmaxx()/6*5,getmaxy()/4*1,"TABLA DE JOC");
            outtextxy(getmaxx()/6,(getmaxy())-getmaxy()/8/2,"INAPOI");
            break;
            }
        case 2:
            {
            setcolor(WHITE);
            outtextxy(getmaxx()/2,50,"CHOOSE YOUR COLORS:");
            outtextxy(getmaxx()/6*1,getmaxy()/4*1,"Player 1");
            outtextxy(getmaxx()/6*3,getmaxy()/4*1,"Player 2");
            outtextxy(getmaxx()/6*5,getmaxy()/4*1,"GAME'S TABLE");
            outtextxy(getmaxx()/6,(getmaxy())-getmaxy()/8/2,"BACK");
            break;
            }
        case 3:
            {
            setcolor(WHITE);
            settextstyle(3,HORIZ_DIR,16);
            outtextxy(getmaxx()/2,50,"CHOISISSEZ LES COULEURS VOULUES:");
            outtextxy(getmaxx()/6*1,getmaxy()/4*1,"JOUEUR 1");
            outtextxy(getmaxx()/6*3,getmaxy()/4*1,"JOUEUR 2");
            outtextxy(getmaxx()/6*5,getmaxy()/4*1,"TABLE DE JEU");
            outtextxy(getmaxx()/6,(getmaxy())-getmaxy()/8/2,"RETOUR");
            break;
            }
    }
}
void deseneazaHighScore()
{
    //Ionita Mihail Catalin && Georgica Marius
        setcolor(INT_COLOR);
        incarcaScor();
        rectangle(10,getmaxy()/8*7 ,getmaxx()/6*2,getmaxy()-10);
        rectangle(getmaxx()-20,getmaxy()/8*7,getmaxx()/16*12,getmaxy()-20);
        setcolor(WHITE);
        settextstyle(3,HORIZ_DIR,15);
        settextstyle(3,HORIZ_DIR,18);
        char s[100],p[100];
        sprintf(s,"%d",meciuriP1);
        sprintf(p,"%d",meciuriP2);
     switch(limbaSist)
    {
        case 1:
            {
                settextstyle(3,HORIZ_DIR,18);
                setcolor(WHITE);
                 outtextxy(getmaxx()/6,(getmaxy())-getmaxy()/8/3,"INAPOI");
                 outtextxy(getmaxx()/10*5,60,"SCORURILE SUNT:");
                 settextstyle(3,HORIZ_DIR,15);
                 outtextxy(getmaxx()-160,getmaxy()-60,"RESETEAZA");
                 outtextxy(getmaxx()/10*3+20,getmaxy()/10*3,"JUCATORUL 1 A CASTIGAT:");
                outtextxy(getmaxx()/10*3+20,getmaxy()/10*3+50,"JUCATORUL 2 A CASTIGAT:");
                 outtextxy (getmaxx()/10*6-40,getmaxy()/10*3,s);
                 outtextxy (getmaxx()/10*6-40,getmaxy()/10*3+50,p);
                break;
            }
        case 2:
            {
                setcolor(WHITE);
                 outtextxy(getmaxx()/6,(getmaxy())-getmaxy()/8/3,"BACK");
                 outtextxy(getmaxx()/10*5,60,"THE SCORES ARE:");
                 settextstyle(3,HORIZ_DIR,15);
                 outtextxy(getmaxx()-160,getmaxy()-60,"RÉINITIALISER");

                 outtextxy(getmaxx()/10*3+20,getmaxy()/10*3,"PLAYER 1 WON:");
                 outtextxy(getmaxx()/10*3+70,getmaxy()/10*3+50,"PLAYER 2 WON:");
                 outtextxy (getmaxx()/10*6-40,getmaxy()/10*3,s);
                 outtextxy (getmaxx()/10*6-40,getmaxy()/10*3+70,p);
                break;
            }
        case 3:
            {
                setcolor(WHITE);
                 outtextxy(getmaxx()/6,(getmaxy())-getmaxy()/8/3,"RETOUR");
                 outtextxy(getmaxx()/10*5,60,"LES OBJECTIFS SONT:");
                 outtextxy(getmaxx()-160,getmaxy()-60,"");
                 settextstyle(3,HORIZ_DIR,15);
                 outtextxy(getmaxx()/10*3+20,getmaxy()/10*3,"JOUEUR 1 MERVEILLEUX:");
                outtextxy(getmaxx()/10*3+20,getmaxy()/10*3+50,"JOUEUR 2 MERVEILLEUX:");
                 outtextxy (getmaxx()/10*6-40,getmaxy()/10*3,s);
                 outtextxy (getmaxx()/10*6-40,getmaxy()/10*3+50,p);
                break;
            }
    }
}

void clickInHIghScore(int x,int y)
{
    //Ionita Mihail Catalin
    if(x>=10 && y>=getmaxy()/8*7 && x<=getmaxx()/6*2 && y<=getmaxy()-10){ //BACK
        cleardevice();
        fereastraActiva=0;
        deseneazaMeniu();
    }
     if(x<=getmaxx()-20 && x>=getmaxx()/16*12 && y>=getmaxy()/8*7 && y<=getmaxy()-20)
     {
        gout.open("data.in",std::fstream::out | std::fstream::app);
        meciuriP1=0;
        meciuriP2=0;
        gout<<endl;
        gout<<meciuriP1<<" "<<meciuriP2;
     }
}
void deseneazaOptiuniDimensiuniTabla(){
    //Ionita Mihail Catalin
    settextstyle(3,HORIZ_DIR,15);
    int laturaMenDim=120;
    setcolor(INT_COLOR);
    rectangle(10,getmaxy()/8*7 ,getmaxx()/6*2,getmaxy()-10);
    int startX=getmaxx()/2-(laturaMenDim+laturaMenDim/2);
    int startY=getmaxy()/16*3+30;
    int val=1;
    //Desenam numpad-ul
    for(int i=0;i<4;i++)
        for(int j=0;j<3;j++)
        {
                char sir[100];
               sprintf(sir,"%d",val);
                rectangle(startX+j*laturaMenDim,startY+i*laturaMenDim,startX+(j+1)*laturaMenDim,startY+(i+1)*laturaMenDim);
                outtextxy(startX+j*laturaMenDim+laturaMenDim/2,startY+i*laturaMenDim+laturaMenDim/2,sir);
                val++;
        }
        setcolor(WHITE);
        //Setam butoanele de 0,SEL si C;
        outtextxy(startX+0*laturaMenDim+laturaMenDim/2,startY+3*laturaMenDim+laturaMenDim/2,"SEL");
        outtextxy(startX+1*laturaMenDim+laturaMenDim/2,startY+3*laturaMenDim+laturaMenDim/2," 0 ");
        outtextxy(startX+2*laturaMenDim+laturaMenDim/2,startY+3*laturaMenDim+laturaMenDim/2,"CL");
        switch(limbaSist)
    {
        case 1:
            {
                setcolor(WHITE);
            outtextxy(getmaxx()/6,(getmaxy())-getmaxy()/8/2,"INAPOI");
            outtextxy(getmaxx()/2,getmaxy()/16,"ALEGETI DIMENSIUNEA TABLEI");
            outtextxy(getmaxx()/2 ,getmaxy()/16*3,"DIMENSIUNEA TABLEI:");
            break;
            }
        case 2:
            {
                setcolor(WHITE);
                outtextxy(getmaxx()/6,(getmaxy())-getmaxy()/8/2,"BACK");
                outtextxy(getmaxx()/2,getmaxy()/16,"CHOOSE GAME SIZE");
                outtextxy(getmaxx()/2 ,getmaxy()/16*3,"GAME SIZE:");
            break;
            }
        case 3:
            {
                setcolor(WHITE);
            outtextxy(getmaxx()/6,(getmaxy())-getmaxy()/8/2,"RETOUR");
            outtextxy(getmaxx()/2,getmaxy()/16,"CHOISISSEZ LA TABLE DIMENSION");
            outtextxy(getmaxx()/2 ,getmaxy()/16*3,"TABLE DIMENSION:");
            break;
            }
    }
}
void seteazaDiametru()
{
    //Ionita Mihail Catalin
    int latime=latimeFer-distantaBorder-3;
    int lungime=lungimeFer-panouVertical-distantaBorder-3;
    if(lungime/(n+1)<latime/(n+1))
    {
            diametru=lungime/(n+1);
            deplasareOriz=latime-(n+1)*diametru;
    }
    else
    {
            diametru=latime/(n+1);
            deplasareVer=lungime-(n+1)*diametru;
    }
}
void startBgMusic()
{
    //Georgica Marius
    PlaySoundA(TEXT("background.wav"),NULL,SND_LOOP | SND_ASYNC);
}
void stopMusic()
{
    //Georgica Marius
    PlaySoundA(0,NULL,NULL);
}
void deseneazaDificultate()
{
    //Ionita Mihail Catalin
    setcolor(INT_COLOR);
      settextstyle(3,HORIZ_DIR,18);
    rectangle(getmaxx()/6*1,getmaxy()/8*1,getmaxx()/6*5,getmaxy()/8*3-getmaxy()/8/2);
    rectangle(getmaxx()/6*1,getmaxy()/8*3,getmaxx()/6*5,getmaxy()/8*5-getmaxy()/8/2);
    rectangle(10,getmaxy()/8*7 ,getmaxx()/6*2,getmaxy()-10);
    switch(limbaSist)
    {
        case 1:
            {
                setcolor(WHITE);
                 outtextxy(getmaxx()/2,getmaxy()/8*3-getmaxy()/8,"NORMAL");
                 outtextxy(getmaxx()/2,getmaxy()/8*5-getmaxy()/8,"AVANSAT");
                 outtextxy(getmaxx()/6,(getmaxy())-getmaxy()/8/2,"INAPOI");
                break;
            }
        case 2:
            {
                setcolor(WHITE);
                outtextxy(getmaxx()/2,getmaxy()/8*3-getmaxy()/8,"NORMAL");
                 outtextxy(getmaxx()/2,getmaxy()/8*5-getmaxy()/8,"ADVANCED");
                 outtextxy(getmaxx()/6,(getmaxy())-getmaxy()/8/2,"BACK");
                break;
            }
        case 3:
            {
                setcolor(WHITE);
                outtextxy(getmaxx()/2,getmaxy()/8*3-getmaxy()/8,"ORDINAIRE");
                 outtextxy(getmaxx()/2,getmaxy()/8*5-getmaxy()/8,"AVANCÉE");
                 outtextxy(getmaxx()/6,(getmaxy())-getmaxy()/8/2,"RETOUR");
                break;
            }
    }
}
void deseneazaTipJoc()
{
    //Ionita Mihail Catalin
    cleardevice();
     settextstyle(3,HORIZ_DIR,18);
    //settextstyle(EUROPEAN_FONT,HORIZ_DIR,25);
    setcolor(INT_COLOR);
    rectangle(getmaxx()/6*1,getmaxy()/8*1,getmaxx()/6*5,getmaxy()/8*3-getmaxy()/8/2);
    rectangle(getmaxx()/6*1,getmaxy()/8*3,getmaxx()/6*5,getmaxy()/8*5-getmaxy()/8/2);
    rectangle(getmaxx()/6*1,getmaxy()/8*5,getmaxx()/6*5,getmaxy()/8*7-getmaxy()/8/2);
    rectangle(10,getmaxy()/8*7 ,getmaxx()/6*2,getmaxy()-10);
    switch(limbaSist)
    {
        case 1:
            {
                setcolor(WHITE);
                settextstyle(3,HORIZ_DIR,17);
            outtextxy(getmaxx()/2,50,"ALEGETI MODUL DE JOC:");
            outtextxy(getmaxx()/2,getmaxy()/8*3-getmaxy()/8,"JUCATOR VS JUCATOR");
            outtextxy(getmaxx()/2,getmaxy()/8*5-getmaxy()/8,"JUCATOR VS COMPUTER");
            outtextxy(getmaxx()/2,getmaxy()/8*7-getmaxy()/8,"OPTIUNI EXTRA");
            outtextxy(getmaxx()/6,(getmaxy())-getmaxy()/8/2,"INAPOI");
            break;
            }
        case 2:
            {
                setcolor(WHITE);
            outtextxy(getmaxx()/2,50,"CHOOSE GAME MODE:");
            outtextxy(getmaxx()/2,getmaxy()/8*3-getmaxy()/8,"PLAYER VS PLAYER");
            outtextxy(getmaxx()/2,getmaxy()/8*5-getmaxy()/8,"PLAYER VS AI");
            outtextxy(getmaxx()/2,getmaxy()/8*7-getmaxy()/8,"EXTRA OPTIONS");
            outtextxy(getmaxx()/6,(getmaxy())-getmaxy()/8/2,"BACK");
            break;
            }
        case 3:
            {
                setcolor(WHITE);
            settextstyle(3,HORIZ_DIR,17);
            outtextxy(getmaxx()/2,50,"CHOISISSEZ LE MODE DE JEU:");
            outtextxy(getmaxx()/2,getmaxy()/8*5-getmaxy()/8,"JOUER VS CALCULATEUR");
            outtextxy(getmaxx()/2,getmaxy()/8*3-getmaxy()/8,"JOUER VS JOUER");
            outtextxy(getmaxx()/2,getmaxy()/8*7-getmaxy()/8,"OPTIONS SUPPLÉMENTAIRES");
            outtextxy(getmaxx()/6,(getmaxy())-getmaxy()/8/2,"RETOUR");
            break;
            }
    }
}
void AiPrimaMiscare()
{
    //Georgica Marius
    int randLin,randCol;
    srand((unsigned)time(&tmp));
    while(jucatorCurent==1)
                    {
                        delay(2);
                            if(a[randLin-1][randCol]==1 && a[randLin+1][randCol]==1 && a[randLin][randCol]==3){
                                deseneazaConexiune(randLin,randCol,1,1);
                                contorCopiiMatrici++;
                                salveazaCopieMatrice();
                                a[randLin][randCol]=4;
                                jucatorCurent=2;
                            }
                            else if(a[randLin][randCol-1]==1 && a[randLin][randCol+1]==1 && a[randLin][randCol]==3){
                                deseneazaConexiune(randLin,randCol,2,1);
                                contorCopiiMatrici++;
                                salveazaCopieMatrice();
                                a[randLin][randCol]=5;
                                jucatorCurent=2;
                            }
                              randLin=rand()%9;
                              randCol=rand()%9;
                                }
                        afisareJucatorCurent();
}
void AiAdv()
{
    //Ionita Mihail Catalin
                int start=0;
                while(start%2==0)
                {
                    srand((unsigned)time(&tmp));
                     delay(5);
                    start=rand()%n;
                }
                    deseneazaConexiune(1,start,1,1);
                    //contorCopiiMatrici++;
                   // salveazaCopieMatrice();
                    a[1][start]=4;
                    jucatorCurent=2;
                    piesaCurentaI=2;
                    piesaCurentaJ=start;
                    counterMisc++;
                   ai[counterMisc].linie=piesaCurentaI;
                   ai[counterMisc].coloana=piesaCurentaJ;
}
void afiseazaMatr(int matr[][200],int n,int m)
{
    for(int i=0;i<=n;i++)
        {for(int j=0;j<=m;j++)
            cout<<matr[i][j]<<" ";
            cout<<endl;
        }
}
void playClick()
{
    //Ionita Mihail Catalin
    if(statusSunete)
    {
    Beep(2000,70);
    //PlaySoundA(TEXT("background.wav"),NULL,SND_LOOP | SND_ASYNC);
    }
}
int main(){
    initControl();
    initializareTabla();
    deseneazaMeniu();
    seteazaDiametru();
    startBgMusic();
    getch();
    closegraph();
    return 0;
}
