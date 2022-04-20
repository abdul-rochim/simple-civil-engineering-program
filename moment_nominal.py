from math import pi

#dimension in mm
#material propertis in MPa
b = 400
h = 600
fy = 400
fc = 30
cover = 40

n = 6
dia_reinf = 19
dia_stirrup = 10
As_tul = n * 0.25*pi*dia_reinf*dia_reinf
d = h - cover - dia_stirrup - 0.5*dia_reinf
a = As_tul*fy/(0.85*fc*b)

#asumption of phi reduction
phi = 0.9

# Mn in kN.m
Mn = As_tul * fy * (d - a/2) /1000000
phi_Mn = phi * Mn

print("Moment Capacity")
print(round(phi_Mn, 2), " kN.m")
