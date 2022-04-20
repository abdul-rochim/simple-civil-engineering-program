// MOMENT CAPACITY CALCULATION OF CONCRETE BEAM
// BASED ON ACI 318-19
// CIVIL ENGINEERING
// by Abdul Rochim
#include <iostream>
#include <iomanip>
#include <assert.h>
#include <cmath>

constexpr double PI = 3.141592653;
constexpr float epsilon_cu = 0.003;

double M_ult = 250; //kN.m
double b = 400; //mm
double h = 600; //mm
double cover = 40; //mm
double dia_reinf = 16; //mm
double num_reinf = 8; //pieces
double dia_tranv = 13; //mm
double fc = 30; //mm
double fy = 420; //mm
double Es = 200000; //mm
const char is_transv_Spiral= 'N'; // "N" for other, "Y" for spiral;

struct data{
    double As;
    double d;
    double a;
    data() : As(0), d(0),a(0){}
    void setAs() { As = num_reinf *.25 * PI * dia_reinf * dia_reinf;}
    void set_d(){
    d = h-cover-dia_tranv-0.5*dia_reinf;}
    void set_a() {
    setAs();
    a = As*fy/(0.85*fc*b);} // concrete block stress
};
struct factor : public data{
    double phi_Mn;   
    double betha1;
    double phi;
    double rho;
    factor() : phi_Mn(0), betha1(0), phi(0), rho(0){}
    factor(double);
    double setBetha1();
    data setPhi();
    double setRho();
};
void Moment_Capacity(){
    std::cout << "Moment Capacity Calculation of Concrete Beam\n";
    std::cout << "based on ACI 2019\n";
    std::cout << "copyright : Abdul Rochim\n";
    std::cout << "****************************\n\n";
    std::cout << "Material Properties:\n";
    std::cout << "b = " << b << " mm\n";
    std::cout << "h = " << h << " mm\n";
    std::cout << "fy = " << fy << " MPa\n";
    std::cout << "dia. of long  reinf. = " << dia_reinf << " mm\n";
    std::cout << "number of long reinf. = " << num_reinf << " pieces\n";
    std::cout << "f'c = " << fc << " MPa\n";
    std::cout << std::fixed << std::setprecision(2);
    factor Mn, Phi, rHO;
    Phi.setPhi();
    data As_reinf, depth, a_block;
    As_reinf.setAs();
    rHO.setRho();
    depth.set_d();
    double rho_applied = As_reinf.As/(b * depth.d);
    double rho_max = 0.025; //for SMRF
    std::cout << "rho maximum = " << rho_max << " [for SMF]" << std::endl;
    std::cout << "rho used = " << rho_applied << std::endl;
    if(rho_applied>rHO.rho && rho_applied< rho_max) std::cout << "OK! rho minimum < rho used < rho max (Non massive Structure)\n";
    else if(rho_applied > rho_max) std::cout << "Not OK! rho used > rho maximum\n";
    else std::cout << "Not OK! rho used < rho minimum\n";
    a_block.set_a();
    Mn.phi_Mn = Phi.phi * As_reinf.As * fy * (depth.d - a_block.a/2) / 1000000;
    std::cout<<"\nMoment ultimate, Mu = " << std::setprecision(2) << M_ult << " kN.m" << std::endl;
    std::cout<<"Moment Capacity, phi Mn = " << Mn.phi_Mn << " kN.m" << std::endl;
    if(Mn.phi_Mn >= M_ult) std::cout << "OK!!! Moment capacity " << " > " << "Moment ultimate " << std::endl;
    else std::cout << "FAIL!!! Moment capacity " << " < " << "Moment ultimate " << std::endl;
}
int main() {

    Moment_Capacity();
    
    return 0;
}
factor::factor(double val){
    phi = val;
}
double factor::setBetha1(){
    assert(fc >= 17);
    betha1 = (fc <= 28)? 0.85 : (fc >= 55)? 0.65 : 0.85 - 0.05/7 * (fc-28);
    std::cout << "betha_1 = " << betha1 << std::endl;
    return betha1;}
double factor::setRho(){
    double rho_min1 = 0.25 * pow(fc, 0.5)/fy;
    double rho_min2 = 1.4 /fy;
    rho = (rho_min1 > rho_min2 ) ? rho_min1 : rho_min2;
    std::cout << "\nrho min req. = " << std::setprecision(5) << rho << std::endl;
    return rho;}
data factor::setPhi(){
    double epsilon_ty = fy/Es;
    
    setBetha1();
    set_a();
    double c = a/ betha1;
    double dt = h - cover - dia_tranv  - 0.5* dia_reinf;
    double epsilon_t = epsilon_cu * (dt-c)/c;
    try{
        //std::cout << "inside throw\n";
        if (is_transv_Spiral != 'N' && is_transv_Spiral != 'Y'){
            throw is_transv_Spiral;
            //std::cout<< "after throw (never executed) \n";
        }
    }
    catch(char is_transv_Spiral){
        //std::cout << "Exception caught\n";
        std::cerr << "YOU ENTER WRONG CHARACTER!!!!!\n";
    }
    //std::cout <<"After catch (will be executed)\n";
    
    if(is_transv_Spiral == 'Y'){ 
    std::cout << "\nUsing Spiral transversal reinforcement\n";
        if(epsilon_t <= epsilon_ty)
            phi = 0.75;
        else if(epsilon_t >= epsilon_ty + 0.003)
            phi = 0.9;
        else
            phi = 0.75+0.15*(epsilon_t - epsilon_ty )/0.003;}
    else if(is_transv_Spiral == 'N'){ 
    std::cout << "\nUsing Non Spiral transversal reinforcement\n";
        if(epsilon_t <= epsilon_ty)
            phi = 0.65;
        else if(epsilon_t >= epsilon_ty + 0.003)
            phi = 0.9;
        else
            phi = 0.65+0.25*(epsilon_t - epsilon_ty )/0.003;}
    else{/*return 1;*/}
    std::cout << std::setprecision(2) << "factor, phi = " << phi << std::endl;
    return factor(phi);
}
