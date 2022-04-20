//Moment - curvature at first yield
public class Calculation{
    float k;
    float dia_long;
    float n_As_tension;
    float n_As_comp;
    float d;
    float d_;
    float b;
    
    float kd;
    float fy;
    float fc_comp;
    float fs_comp;
    float Es;
    float Ec;
        
    Calculation () {};
    Calculation (float fy, float Es, float Ec, float dia_long, float n_As_tension, float n_As_comp, float b, float d, float d_){
        this.fy = fy;
        this.Es = Es;
        this.Ec = Ec;
        this.dia_long = dia_long;
        this.n_As_tension = n_As_tension;
        this.n_As_comp = n_As_comp;
        this.b = b;
        this.d = d;
        this.d_ = d_;
    }
    public float k(){
        SteelReinf s = new SteelReinf(this.dia_long);
        s.As_tension(this.n_As_tension);
        s.As_compression(this.n_As_comp);
        float rho_tension = s.As/ (b*d);
        float rho_comp = s.As_comp/ (b*d);
        
        MatProp m = new MatProp(Es, Ec, fy);
        float n = m.n();
        return (float)Math.pow((float)Math.pow((rho_tension+rho_comp)*n, 2.f) + 2.f *(rho_tension+rho_comp*d_/d)*n, 0.5f) - (rho_tension+rho_comp)*n;
    }
    
    public void kd(){
        kd = k() * d;
    }
    public float epsilon_conc_comp(){
        MatProp m = new MatProp(Es, Ec, fy);
        kd();
        float epsilon_s = m.epsilon_tension_yield();
        return epsilon_s * kd / (d - kd);
    }
    
    public float epsilon_s_comp(){
        MatProp m = new MatProp(Es, Ec, fy);
        kd();
        float epsilon_s = m.epsilon_tension_yield();
        return epsilon_s * ( kd - d_) / (d - kd);
    }
    
    public float Moment_first_yield(){
        fc_comp = epsilon_conc_comp() * Ec;
        fs_comp = epsilon_s_comp() * Es;
        kd();
        float Cc = 0.5f * fc_comp * b * kd;
        
        SteelReinf s = new SteelReinf(this.dia_long);
        s.As_tension(n_As_tension);
        s.As_compression(n_As_comp);
        float Cs = s.As_comp * fs_comp;
        
        float y_ = (Cs * d_ + Cc * kd/3) / (Cc + Cs);
        float jd = d - y_;
        
        return s.As * fy * jd;
    }
    
    public float curvature(){
        MatProp m = new MatProp(Es, Ec, fy);
        return m.epsilon_tension_yield()/ (d - kd);
    }
}

public class SteelReinf{
    float dia_long;
    float As;
    float As_comp;
    
    SteelReinf(float dia){
        this.dia_long = dia;
    }    
    public void As_tension(float n_As_tension){
        As = n_As_tension * 0.25f * (float)Math.PI * (float)Math.pow(dia_long, 2);
        //return As;
    }
    public void As_compression(float n_As_comp){
        As_comp = n_As_comp * 0.25f * (float)Math.PI * (float)Math.pow(dia_long, 2);
        //return As_comp;
    }
}

public class MatProp{
    float Es;
    float Ec;
    float fy;
    //float fc;
    
    MatProp (float Es, float Ec, float fy){
        this.Es = Es;
        this.Ec = Ec;
        this.fy = fy;
    }
    public float epsilon_tension_yield(){
        return fy/Es;
    }
    public float n(){
        return Es/Ec;
    }  
}

public class Program
{
    public static void main(String[] args) {
        //data
        float fy = 40000; //psi
        float Es = 29000000; // psi
        float Ec = 3200000; // psi 
        float dia_long = 1; // in.
        float n_As_tension = 4; // pieces
        float n_As_comp = 2; // pieces
        float b = 10; // in.
        float d = 23; // in.
        float d_ = 2; // in.
        Calculation calc = new Calculation(fy, Es, Ec, dia_long, n_As_tension, n_As_comp, b, d, d_);
        System.out.println("Bending Moment after crack at First Yield = " + calc.Moment_first_yield() + " lb.in");
        System.out.println("Curvature at First Yield = " + calc.curvature() + " rad/in");

    }
}
