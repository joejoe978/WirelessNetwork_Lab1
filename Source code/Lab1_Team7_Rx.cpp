  // for opencv 2.9 use the following 2 headers//
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "cv.h"
#include "math.h"
/*
#define LHU 374 //DEFAULT = 0
#define LHD 270 //DEFAULT = 0

#define RHU 374 //DEFAULT = 0
#define RHD 270 //DEFAULT = 0

#define LWL 341 //DEFAULT = 0
#define LWR 502 //DEFAULT = 0

#define RWL 600 //DEFAULT = 0
#define RWR 250 //DEFAULT = 0
*/
#define LRT 0 //DEFAULT = 255
#define LGT 0 //DEFAULT = 255
#define LBT 255  //DEFAULT = 255

#define RRT 0 //DEFAULT = 255
#define RGT 0 //DEFAULT = 255
#define RBT 255  //DEFAULT = 255

#define MW 30
#define MH 180
#define MHr 0.2 //DEFAULT = 1

#define XSH 50
#define YSH 10

#define DIFF 35
using namespace cv;
using namespace std;

static float fwidth = 1920;
static float fhight = 1080;
int cnt = 0;


int LHU,LHD,RHU,RHD,LWL,LWR,RWL,RWR;

void find_2balls(IplImage *im, int *colSum, int *itSum) //init
{
    int LX,RX,ITR,ITL;
    int max_L = 0, max_R = 0, temp = -1;
    //cout << "@";
    memset(colSum , 0, sizeof(int)*(im->width));
    memset(itSum , 0, sizeof(int)*((im->width)+(im->height)));
    //cout << "@";
    for(int i = 0; i < im->width; i++)
    {
        for(int j = 0; j < im->height; j++)
        {
            int b = ((uchar *)(im->imageData + j*im->widthStep))[i*im->nChannels + 0];
            int g = ((uchar *)(im->imageData + j*im->widthStep))[i*im->nChannels + 1];
            int r = ((uchar *)(im->imageData + j*im->widthStep))[i*im->nChannels + 2];
            //if (b >= LBT) b = LBT;
            //else b = 0;
            if (g >= LGT) g = LGT;
            else g = 0;
            if (r >= LRT) r = LRT;
            else r = 0;
            colSum[i] += b + g + r;
            itSum[i+j] += b + g + r;
        }
    }
    //cout << "@";
    for(int i = 0; i < (im->width)/2; i++)
    {
        if(temp < colSum[i])
        {
            temp = colSum[i];
            LX = i;
        }
    }
    temp = -1;
    for(int i = (im->width)/2; i < (im->width); i++)
    {
        if(temp < colSum[i])
        {
            temp = colSum[i];
            RX = i;
        }
    }
    temp = -1;
    for(int i = 0; i < ((im->width)+(im->height))/2; i++)
    {
        if(temp < itSum[i])
        {
            temp = itSum[i];
            ITL = i;
        }
    }
    temp = -1;
    for(int i = ((im->width)+(im->height))/2; i < ((im->width)+(im->height)); i++)
    {
        if(temp < itSum[i])
        {
            temp = itSum[i];
            ITR = i;
        }
    }
    int LY = ITL - LX, RY = ITR - RX;
    LHU = LY - MH + YSH;
    LHD = (im->height) - (LY + MH) - YSH;
    RHU = RY - MH + YSH;
    RHD = (im->height) - (RY + MH) - YSH;
    LWL = LX - MW + XSH;
    LWR = (im->width)/2 - (LX + MW) - XSH;
    RWL = (RX - MW) - (im->width)/2 + XSH;
    RWR = (im->width) - (RX + MW) - XSH;
/*
    cout << "LHU = " << LHU << ", "
         << "LHD = " << LHD << ", "
         << "RHU = " << RHU << ", "
         << "RHD = " << RHD << ", "
         << "LWL = " << LWL << ", "
         << "LWR = " << LWR << ", "
         << "RWL = " << RWL << ", "
         << "RWR = " << RWR << ", " << endl;
*/
    return;
}



int count_stripe(IplImage *im, int *difference, int *rowSumLeft, int *rowSumRight, float& ll, float& rr){

    int i,j,k,l,t,r,g,b,kl,kll,p,pl,pll;
    float m;
    memset(difference, 0, sizeof(int)*(im->height)/2);
    memset(rowSumLeft , 0, sizeof(int)*(im->height));
    memset(rowSumRight , 0, sizeof(int)*(im->height));

    for(i = 0; i < (im->height); i++)
    {
        for(j = LWL; j < ((im->width)/2 - LWR) && i >= LHU && i < (im->height) - LHD; j++)
        {
            b = ((uchar *)(im->imageData + i*im->widthStep))[j*im->nChannels + 0];
            g = ((uchar *)(im->imageData + i*im->widthStep))[j*im->nChannels + 1];
            r = ((uchar *)(im->imageData + i*im->widthStep))[j*im->nChannels + 2];
            //if (b >= LBT) b = LBT;
            //else b = 0;
            if (g >= LGT) g = LGT;
            else g = 0;
            if (r >= LRT) r = LRT;
            else r = 0;
            t = b + g + r;
            rowSumLeft[i] += t;
        }
        //cout <<" ["<< rowSumLeft[i] << "] ";
        for(j = RWL+(fwidth/2); j < (im->width - RWR) && i >= RHU && i < (im->height) - RHD; j++)
        {
            b = ((uchar *)(im->imageData + i*im->widthStep))[j*im->nChannels + 0];
            g = ((uchar *)(im->imageData + i*im->widthStep))[j*im->nChannels + 1];
            r = ((uchar *)(im->imageData + i*im->widthStep))[j*im->nChannels + 2];
            //if (b >= RBT) b = RBT;
            //else b = 0;
            if (g >= RGT) g = RGT;
            else g = 0;
            if (r >= RRT) r = RRT;
            else r = 0;
            t = b + g + r;
            rowSumRight[i] += t;
        }
        //cout<<rowSumRight[i] << " ";
    }
    m = 0;
    for(k = 0;k<(im->height - (LHU + LHD))/2;k++) {
        for (i = LHU; i<(im->height - LHD) - (im->height - (LHU + LHD))/2; i++)
            difference[k] += abs(rowSumLeft[i] - rowSumLeft[i + k]);
        difference[k] /= ((im->height - (LHU + LHD)) - k + 1); //should float
        m += difference[k];
        kl = k;
        while(difference[kl] == difference[k] && kl >= 0) kl--;
        kll = kl;
        while(difference[kll] == difference[kl] && kll >= 0) kll--;
        if(difference[kll]>difference[kl] && difference[k]>difference[kl] && k > 1 && difference[kl] < m/((k+1)))
            break;
    }
    m = 0;
    memset(difference, 0, sizeof(int)*(im->height)/2);
    for(p = 0;p<(im->height - (RHU + RHD))/2;p++) {
        for (i = RHU; i<(im->height - RHD) - (im->height - (RHU + RHD))/2; i++)
            difference[p] += abs(rowSumRight[i] - rowSumRight[i + p]);
        difference[p] /= ((im->height - (RHU + RHD)) - p + 1); //should float
        m += difference[p];
        pl = p;
        while(difference[pl] == difference[p] && pl >= 0) pl--;
        pll = pl;
        while(difference[pll] == difference[pl] && pll >= 0) pll--;
        //cout<<p << " " << difference[p] << endl;
        if(difference[pll]>difference[pl] && difference[p]>difference[pl] && p > 1 && difference[pl] < m/((p+1)))
            break;
    }

    ll = fhight/(kl)*30*1.637716;
    rr = fhight/(pl)*30*1.637716;


    //cout << setprecision(2) << setiosflags(ios::fixed) << ll << "  ...   " <<rr <<endl;

    k = round((ll - 350)/20);
    p = round((rr - 350)/20);

    return ((k*16) + p);   /// k  p change
}

//if one ball crush, whole frame drop(return true, else return false)
bool isDropable(IplImage *im, int *difference, int *rowSumLeft, int *rowSumRight)
{
    char u,d;
    float ull,urr,dll,drr; // (u/d) der (ll/rr)

    LHD += round(MH*MHr);
    RHD += round(MH*MHr);

    u = count_stripe(im, difference, rowSumLeft, rowSumRight, ull, urr);

    LHD -= round(MH*MHr);
    RHD -= round(MH*MHr);

    LHU += round(MH*MHr);
    RHU += round(MH*MHr);

    d = count_stripe(im, difference, rowSumLeft, rowSumRight, dll, drr);

    LHU -= round(MH*MHr);
    RHU -= round(MH*MHr);

    if (abs(ull - dll) <= DIFF && abs(urr - drr) <= DIFF) return false;
    else return true;
}

int main()
{
    VideoCapture cap("D:/研究所資料/wireless network/project1/測資/新/new 54/IMG_4318.MOV");
    if ( !cap.isOpened() )  // if not success, exit program
    {
         cout << "Cannot open the video file" << endl;
         return -1;
    }

    int B = 0;

    Mat frame;
    cap.read(frame);

    IplImage* img1;
    img1 = cvCreateImage(cvSize(frame.cols,frame.rows),8,3);
    IplImage ipltemp=frame;
    cvCopy(&ipltemp,img1);
    int *difference = (int *) calloc((img1->height)/2,sizeof(int));
    int *rowSumLeft = (int *) calloc((img1->height),sizeof(int));
    int *rowSumRight = (int *) calloc((img1->height),sizeof(int));
    int *colSum = (int *) calloc((img1->width),sizeof(int));
    int *itSum = (int *) calloc((img1->width)+(img1->height),sizeof(int));
    bool d;
    char c = 127,cb; //c(after) & c(before)
    float ll,rr;

    //if ((char)count_stripe(img1, difference, rowSumLeft, rowSumRight) == 'B') B++;
    find_2balls(img1, colSum, itSum);
    d = isDropable(img1, difference, rowSumLeft, rowSumRight);
    cb = c;
    c = (char)count_stripe(img1, difference, rowSumLeft, rowSumRight, ll, rr);
    //if (!d && (ll <= 670) && (rr <= 670) && (ll >=330) && (rr >=330) && cb != c) cout << c;
    if (!d && (ll <= 500) && (rr <= 660) && (ll >=380) && (rr >=340)) cout << c;
    ++cnt;
    //cout<< " " << d<<": " << c << "("<< ++cnt <<")   " << endl;
    //cout << "----------------------------------------------------"<< endl;

    cvReleaseImage(&img1);

    while(1)
    {


        bool bSuccess = cap.read(frame); // read a new frame from video

        if (!bSuccess) //if not success, break loop
        {
            cout << endl;
            //cout << "Cannot read the frame from video file" << endl;
            break;
        }


        IplImage* img1;
        img1 = cvCreateImage(cvSize(frame.cols,frame.rows),8,3);
        IplImage ipltemp=frame;
        cvCopy(&ipltemp,img1);

        //if(cnt % 2 == 1) find_2balls(img1, colSum, itSum);
        d = isDropable(img1, difference, rowSumLeft, rowSumRight);
        cb = c;
        c = (char)count_stripe(img1, difference, rowSumLeft, rowSumRight, ll, rr);
        //if (!d && (ll <= 670) && (rr <= 670) && (ll >=330) && (rr >=330) && cb != c) cout << c << " cnt= " <<cnt+1<< endl;
        if (!d && (ll <= 500) && (rr <= 660) && (ll >=380) && (rr >=340) ) cout << c ;//<< " cnt= " <<cnt+1<< endl;
        ++cnt;
        //cout<< " " << d<<": " << c << "("<< ++cnt <<")   " << endl;
        //cout << "----------------------------------------------------"<< endl;



        cvReleaseImage(&img1);
        if(waitKey(30) == 27) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
        {
                cout << "esc key is pressed by user" << endl;
                break;
        }

        //if(cnt==152) break;

    }
    free(difference);
    free(rowSumLeft);
    free(rowSumRight);

    //cout<<"count: "<<cnt<<endl;
    //cout<<"B: "<<B<<endl;
    waitKey(0);
    return 0;
}
