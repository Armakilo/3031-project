float angleL;
float angle2;
float angle3;

float h = 0;
float R = 0;
float A;
float a;
float c;

#define L 0.22

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  A = sqrt(h^2 + R^2);
  angleL = arctan(h/R);

  a = arccos(((2*(L^2))-A^2)/(4*L));
  c = arcsin((L*sin(a))/A);

  angle2 = arcsin(L*sin(a)/A) + angleL;
  angle3 = 180 - a - angle2;
}
