#include "cmath"
#include <QPainter>
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

#ifndef STAR_H
#define STAR_H

int numStars;
double systemRadius, distConnect;
const int dim = 2;

const int borderMassC = 10;
const double G = 6.67408e-11;
double dt = 10000;
const double massSun   = 1.98892e30,
             massJup   = 1898.6e24,
             massUran  = 86.832e24,
             massEarth = 5.9742e24,
             massVenus = 4.867e24;
const double borderMass[] = {borderMassC*massEarth, borderMassC*massUran, borderMassC*massJup, borderMassC*massSun};
const QColor colStar[] = {Qt::cyan, Qt::red, Qt::magenta, Qt::yellow, Qt::white};
const int nColor = sizeof(colStar) / sizeof(colStar[0]);

class star{
public:

    static int starCounter;
    double x[dim];
    double v[dim];
    double m;
    double f[dim];
    QColor col;
    star(double *coord, double *speed, double mass);
    ~star(){starCounter--;}
};
int star::starCounter = 0;

star::star(double *coord, double *speed, double mass){
    for(int k = 0; k < dim; ++k){
        x[k] = coord[k];
        v[k] = speed[k];
    }
    m = mass;
    col = colStar[nColor-1];
    for(int i = 0; i < nColor-1; ++i){
        if(m <= borderMass[i]){
            col = colStar[i];
            break;
        }
    }
    starCounter++;
}
class galaxy{
public:
    int num;
    star **stars;
    friend ostream& operator<< (ostream &out, const galaxy *galaxy1){
        for(int i = 0; i < galaxy1->num; i++){
            if(galaxy1->stars[i]){
                out << "ID    " << i
                    << "    Mass    " << galaxy1->stars[i]->m
                    << "    XY    " << galaxy1->stars[i]->x[0] << "    " << galaxy1->stars[i]->x[1]
                    <<endl;
            }

        }

        return out;
    };
    galaxy(int n = numStars):num(n){
        stars = new star*[num];
        double x1[dim] = {0}, v1[dim] = {0};
        stars[0] = new star(x1, v1, massSun);
        double rad;
        for(int i = 1; i < num; ++i){
            rad = 0;
            double R = rand() * systemRadius / RAND_MAX,
            fi  = (2 * M_PI * rand()) / RAND_MAX,
            theta = (M_PI * rand()) / RAND_MAX;
            x1[0] = R  * cos(fi);
            x1[1] = R  * sin(fi);
            if(dim == 3){
                x1[0] *= sin(theta);
                x1[1] *= sin(theta);
                x1[3] = R * cos(theta);
            }
            for(int k = 0; k < dim; ++k){
                rad += x1[k] * x1[k];
            }
            double absV = sqrt(G * stars[0]->m / sqrt(rad)), alpha = (2 * M_PI * rand()) / RAND_MAX;
            v1[0] =  absV * sin(fi);
            v1[1] = -absV * cos(fi);
            stars[i] = new star(x1, v1, massEarth / num * (6 * i));
        }
    };
    ~galaxy(){delete[] stars;};
    double border[dim];

    void move(){
        double dist;
        double dCoord[dim];
        for(int i = 0; i < num; ++i){ // force nullification
            for(int k = 0; k < dim; ++k){
                if(stars[i]){
                    stars[i]->f[k] = 0;
                }
            }
        }
        for(int i = 0; i < num; i++){
            if(stars[i]){
                for(int j = i + 1; j < num; j++){
                    if(i != j && stars[j]){
                        dist = 0;
                        for(int k = 0; k < dim; ++k){
                            dCoord[k] = stars[i]->x[k] - stars[j]->x[k];
                            dist += dCoord[k] * dCoord[k];
                        }
                        if(sqrt(dist) < distConnect){
                            double tmpM = stars[i]->m + stars[j]->m, tmpX[dim], tmpV[dim];
                            for(int k = 0; k < dim; ++k){
                                tmpX[k] = (stars[i]->x[k] * stars[i]->m + stars[j]->x[k] * stars[j]->m)/tmpM;
                                tmpV[k] = (stars[i]->v[k] * stars[i]->m + stars[j]->v[k] * stars[j]->m)/tmpM;
                            }
                            delete stars[j];
                            stars[j] = nullptr;
                            stars[i]->m = tmpM;
                            for(int k = 0; k < dim; ++k){
                                stars[i]->x[k] = tmpX[k];
                                stars[i]->v[k] = tmpV[k];
                                for(int l = 0; l < nColor-1; ++l){              //Изменение цвета в зависимости от массы
                                    if(stars[i]->m <= borderMass[l]){
                                        stars[i]->col = colStar[l];
                                        break;
                                    }
                                }

                            }
                        }
                    }
                }
            }
        }
        for(int i = 0; i < num; i++){
            if(stars[i]){
                for(int j = i + 1; j < num; j++){
                    if(i != j && stars[j]){
                        dist = 0;
                        for(int k = 0; k < dim; ++k){
                            dCoord[k] = stars[i]->x[k] - stars[j]->x[k];
                            dist += dCoord[k] * dCoord[k];
                        }
                        for(int k = 0; k < dim; ++k){
                            double tmp = G * stars[i]->m * stars[j]->m / dist;
                            stars[i]->f[k] -= tmp * dCoord[k] / sqrt(dist);
                            stars[j]->f[k] += tmp * dCoord[k] / sqrt(dist);
                        }
                    }
                }
            }
        }
        for(int i = 0; i < num; ++i){
            if(stars[i]){
                for(int k = 0; k < dim; ++k){
                    stars[i]->v[k] += dt * stars[i]->f[k] / stars[i]->m;
                }
                for(int k = 0; k < dim; ++k){
                    stars[i]->x[k] += dt * stars[i]->v[k];
                }
            }
        }
    }
};



#endif // STAR_H
