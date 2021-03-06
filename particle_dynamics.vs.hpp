/* File generated with Shader Minifier 1.1.4
 * http://www.ctrl-alt-test.fr
 */
#ifndef PARTICLE_DYNAMICS_VS_HPP_
# define PARTICLE_DYNAMICS_VS_HPP_

const char *particle_dynamics_vs =
 "#version 330\n"
 "float s(float v,float m)"
 "{"
   "float i=m/4.*60.,f=15.,c=.8;"
   "return smoothstep(-f/2.,-f*(1.-c)/2.,-abs(v-i-f*.5));"
 "}"
 "vec3 s(float m)"
 "{"
   "float v=m+2.;"
   "vec3 f=vec3(v,v*v,v*v*v),c=fract(f*222.)+vec3(2.);"
   "return fract(f*c.yzx*c.zxy);"
 "}"
 "float n(float m)"
 "{"
   "return m/44100;"
 "}"
 "float v(float m)"
 "{"
   "return 1.-smoothstep(0.,4.,m);"
 "}"
 "float f(float m)"
 "{"
   "float f=60.-m;"
   "return 1.-smoothstep(0.,4.,f);"
 "}"
 "vec3 i(float v)"
 "{"
   "float m=max(s(v,1.),s(v,3.));"
   "return vec3(sin(6.28319*v*.5)*.2*m,abs(sin(6.28319*v*.5))*.6*m,0.);"
 "}"
 "float f(vec3 v,float m)"
 "{"
   "vec3 f=pow(abs(v),vec3(m));"
   "return pow(f.x+f.y+f.z,1/m);"
 "}"
 "float i(vec3 v,float m)"
 "{"
   "return length(v)-m;"
 "}"
 "float n(vec3 v,float m)"
 "{"
   "return length(v.yz)-m;"
 "}"
 "float v(vec3 v,float m)"
 "{"
   "return length(v.xz)-m;"
 "}"
 "vec3 m(vec3 m,float v)"
 "{"
   "return m-i(v);"
 "}"
 "vec3 x(vec3 v,float f)"
 "{"
   "vec3 c=m(v,f);"
   "return vec3(abs(c.x),c.yz);"
 "}"
 "float a(vec3 v,float m)"
 "{"
   "vec3 f=x(v,m);"
   "f-=vec3(.2,.2,.42);"
   "vec3 c=mat3(cos(.3)*cos(-.4),-sin(.3)*cos(-.4),-sin(-.4),sin(.3),cos(.3),0.,cos(.3)*sin(-.4),-sin(.3)*sin(-.4),cos(-.4))*f;"
   "return max(max(-c.x,c.x-.15),n(c,.014));"
 "}"
 "float c(vec3 v,float m)"
 "{"
   "vec3 f=x(v,m);"
   "return min(a(v,m),i(f-vec3(.23,.04,.4),.09));"
 "}"
 "float o(vec3 f,float c)"
 "{"
   "vec3 n=m(f,c),a=x(f,c);"
   "float o=abs(sin(c*3.)*3.14159*.5*s(c,1));"
   "vec3 l=mat3(1.,0.,0.,0.,cos(o),sin(o),0.,-sin(o),cos(o))*(n-vec3(0.,cos(acos(.9))*.5,sin(-acos(.9))*.5))+vec3(0.,cos(acos(.9))*.5,sin(-acos(.9))*.5);"
   "float p=min(min(max(i(n,.5),n.y-cos(acos(.9))*.5),max(v(n,.15),max(n.y-cos(acos(.9))*.5-.03,-n.y+cos(acos(.9))*.5))),max(max(i(l,.5),-(l.y-cos(acos(.9))*.5)),-max(v(l,.15),max(l.y-cos(acos(.9))*.5-.03,-l.y+cos(acos(.9))*.5-.1))));"
   "return min(p,i(a-vec3(.045,-.1,.49),.06));"
 "}"
 "float t(vec3 m,float v)"
 "{"
   "vec3 c=m-vec3(0.,-.5,0.),n=fract(c)*2.-1.;"
   "return max(f(n,8.)-1.05,c.y);"
 "}"
 "float l(vec3 v,float m)"
 "{"
   "return min(min(o(v,m),c(v,m)),t(v,m));"
 "}struct material{vec3 diffuse;vec3 ambient;};"
 "material p(vec3 v,float m)"
 "{"
   "float f=t(v,m),n=o(v,m),a=c(v,m);"
   "if(n<f&&n<a)"
     "return material(vec3(.9,.8,.3),vec3(.3,.25,.1));"
   "else"
     " if(a<f)"
       "return material(vec3(.03),vec3(.005));"
     "else"
       " return material(vec3(.1,.8,.1),vec3(.23,.23,.3));"
 "}"
 "vec3 e(vec3 v,float m)"
 "{"
   "const float f=.0001;"
   "return normalize(vec3(l(v+vec3(f,0.,0.),m)-l(v+vec3(-f,0.,0.),m),l(v+vec3(0.,f,0.),m)-l(v+vec3(0.,-f,0.),m),l(v+vec3(0.,0.,f),m)-l(v+vec3(0.,0.,-f),m)));"
 "}"
 "uniform float sample_count;"
 "layout(location=0)in vec3 in_crnt_pos[5];"
 "layout(location=5)in vec3 in_prev_pos;"
 "out vec3 out_position;"
 "void main()"
 "{"
   "float m=n(sample_count),f=float(gl_VertexID/16)/20000.;"
   "int c=gl_VertexID%16;"
   "float a=f*4.98319+.65,o=acos(s(f).x*.9),x=.5*sin(o);"
   "vec3 r=vec3(x*sin(a),.51*cos(o),x*cos(a))+i(m);"
   "r=mix(r,r+vec3(sin(a+m*3.14159*.3)*1.2,.1*sin(m*9.+f*13.),cos(a+m*3.14159*.3)*1.2),s(m,2.));"
   "float p=.02+.003*sin(a*8.);"
   "if(c==0)"
     "{"
       "out_position=r;"
       "return;"
     "}"
   "vec3 t=vec3(sin(o)*sin(a),cos(o),sin(o)*cos(a));"
   "if(v(m)>0.)"
     "{"
       "out_position=r+t*p*float(c);"
       "return;"
     "}"
   "out_position=in_crnt_pos[2];"
   "vec3 g=in_crnt_pos[2]-in_prev_pos;"
   "out_position+=g*.91+vec3(0.,-.0004,0.);"
   "vec3 u=normalize(out_position-in_crnt_pos[1]);"
   "out_position=in_crnt_pos[1]+u*p;"
   "if(c==1)"
     "out_position=in_crnt_pos[1]+t*p;"
   "else"
     " if(c==15)"
       "{"
         "vec3 y=in_crnt_pos[1]-in_crnt_pos[0],z=out_position-in_crnt_pos[1],h=(y-z)/3.;"
         "out_position+=h*.5*.5;"
       "}"
     "else"
       " if(c==14)"
         "{"
           "vec3 y=in_crnt_pos[1]-in_crnt_pos[0],z=out_position-in_crnt_pos[1],h=in_crnt_pos[3]-out_position,b=(y-z)/3.+(h-z)*2/3.;"
           "out_position+=b*.5*.5;"
         "}"
       "else"
         "{"
           "vec3 y=in_crnt_pos[1]-in_crnt_pos[0],z=out_position-in_crnt_pos[1],h=in_crnt_pos[3]-out_position,b=in_crnt_pos[4]-in_crnt_pos[3],V=(y-z)/3.+(h-z)*2/3.+(h-b)/3.;"
           "out_position+=V*.5*.5;"
         "}"
   "u=normalize(out_position-in_crnt_pos[1]);"
   "out_position=in_crnt_pos[1]+u*p;"
   "float z=l(out_position,m);"
   "const float y=.01;"
   "if(z<y)"
     "{"
       "vec3 h=e(out_position,m);"
       "out_position+=(y-z)*h;"
     "}"
 "}";

#endif // PARTICLE_DYNAMICS_VS_HPP_
