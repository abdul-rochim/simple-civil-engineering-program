//Shear Strength of Reinforced Concrete Beam
#include <iostream>
#include <iomanip>
#include <cmath>
//#include <numbers>
//using namespace std::numbers;

inline constexpr auto b = 400; //mm
inline constexpr auto h = 600; //mm
inline constexpr auto cover = 40; //mm

inline constexpr auto fc = 30; //MPa
inline constexpr auto fy = 420; //MPa

inline constexpr auto pi {0.75};

inline constexpr auto n = 4; //leg
inline constexpr auto s = 150; //mm
inline constexpr auto dia_stir = 10; //mm
inline constexpr auto dia_long = 16; //mm

template <typename T>
struct Type;

template<>
struct Type<int>{
    using type = int;
};

template<>
struct Type<float>{
    using type = float;
};

template<>
struct Type<double>{
    using type = double;
};

template <typename T, typename dataType = typename Type<T>::type>
struct Shear_strength{
    dataType Vs(){
    T Av = n*M_PI_4*std::pow(dia_stir, 2);
    const T d = h - cover - dia_stir - 0.5*dia_long;
    return (Av*fy*d/s)/1000; 
    }
    dataType Vc(){
    const T d = h - cover - dia_stir - 0.5*dia_long;
    return (1./6*std::sqrt(fc)*b*d)/1000;
    }
    dataType pi_Vn() { return (pi * (Vs() + Vc()));};
};

template <typename T, typename dataType = typename Type<T>::type>
void shearStrength_view(){
    Shear_strength<T, dataType> s;
    std::cout << s.pi_Vn() << " kN\n";
}

int main()
{
    using dataType = typename Type<float>::type; 
    using dataType_S = Shear_strength<float, dataType>;
    
    std::cout << "Calling using object\n";
    dataType_S s;
    std::cout << s.pi_Vn() << " kN\n";
    
    std::cout << "\nCalling using function\n";
    shearStrength_view<float, dataType>();

    return 0;
}
