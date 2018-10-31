//
//  main.cpp
//  hw3
//
//  Created by Ashwin Zalte on 10/21/18.
//  Copyright © 2018 Ashwin Zalte. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <random>
#include <math.h>
using namespace std;
#define M 100
#define assets 947

//ifstream myfile ("/Users/ashwinzalte/Desktop/3rd sem/Application Programming for FE/Homework/3/test.txt");

float sigmoid (float a)
{
    float sigmoid1=1/(1+exp(-a));
    return sigmoid1;
}


int main()
{
    ifstream file;
    file.open("russell_prices.txt");
    double russell[assets+1][505];
    double ret_mat[assets+1][504];
    
    int i,j,k;
    // *** READING THE TXT FILE
    if(file.is_open())
    {
        while(!file.eof())
        {
            for( i = 1; i <= assets; i++)
            {
                for(j = 1; j <= 504; j++)
                {
                    file >> russell[i][j];
                }
            }
        }
    }
    file.close();
    
    // *** CALCULATING THE RETURNS MATRIX
    for(i = 1; i <= assets; i++)
    {
        for(j = 1; j <= 503; j++)
        {
            ret_mat[i][j]= ((russell[i][j+1])/(russell[i][j]))-1;
            //cout << ret_mat[i][j]<<"  ";
        }
        //cout<<endl;
    }
    
    //int layer_dims[4];
    //layer_dims[1]=947; layer_dims[2]=m; layer_dims[3]=m; layer_dims[3]=947;
    //cout<< layer_dims[1];
    
    // ***  INITIALIZE_PARAMETERS_DEEP
    static float W1[M+1][assets + 1],W2[M+1][M+1],W3[assets + 1][M+1];
    static float b1[M+1],b2[M+1],b3[assets + 1];
    
    default_random_engine generator; //seed
    normal_distribution<float> distribution(0, 1); //mean followed by stdiv
    
    //Initialize W1 & b1
    for( i = 1; i <=M ; i++)
    {
        for(j = 1; j <= assets; j++)
        {
            W1[i][j] =0.01* distribution(generator); //Generate numbers;
            //cout<<W1[i][j]<<"   ";
        }//cout<<endl;
        b1[i]=0;
    }
    //Initialize W2 & b2
    for( i = 1; i <=M ; i++)
    {
        for(j = 1; j <= M; j++)
        {
            W2[i][j] =0.01* distribution(generator); //Generate numbers;
            //cout<<W2[i][j]<<"   ";
        }
        b2[i]=0;//cout<<endl;
    }
    //Initialize W3 & b3
    for( i = 1; i <= assets; i++)
    {
        for(j = 1; j <= M; j++)
        {
            W3[i][j] =0.01* distribution(generator); //Generate numbers;
        }
        b3[i]=0;
    }
    // A- Sigmoid function
    static float A[assets + 1][243];//B2[101],B3[947];
    for( i = 1; i <= assets; i++)
    {
        for(j = 1; j <= 242; j++)
        {
            A[i][j] =sigmoid(ret_mat[i][j]);
        }
    }
    
    // L_MODEL_FORWARD : INPUT = B1 parameters (W1,W2,W3,b1,b2,b3)
    // LINEAR_ACTIVATION_FORWARD
    //LINEAR_FORWARD
    
    static float A_prev[assets + 1][243];
    for( i = 1; i <= assets; i++)
    {
        for(j = 1; j <= 242; j++)
        {
            A_prev[i][j] =A[i][j];
        }
    }
    static float Z1_inter[101][243],Z1[101][243], A1[101][243];
   // matrix multiplication
    for(i = 1; i <=100 ; i++)
    {
        for(j = 1; j <= 242; j++)
        {
            Z1_inter[i][j]=0;Z1[i][j]=0;
        }
    }
    
    for(i = 1; i <= 100; i++)
        for(j = 1; j <= 242; j++)
            for(k = 0; k <= 947; k++)
            {
                Z1_inter[i][j] += (W1[i][k] * A_prev[k][j]);
            }
    
    for(j = 1; j <=242 ; j++)
    {
        for(i= 1; i <=100; i++)
        {
            Z1[i][j]=Z1_inter[i][j]+b1[i];
        }
    }
    
    for( i = 1; i <=100 ; i++)
    {
        for(j = 1; j <= 242; j++)
        {
            A1[i][j] =sigmoid(Z1[i][j]);
        }
    }
    
     cout<<"1";
    return 0;
}

