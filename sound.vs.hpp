/* File generated with Shader Minifier 1.1.4
 * http://www.ctrl-alt-test.fr
 */
#ifndef SOUND_VS_HPP_
# define SOUND_VS_HPP_

const char *sound_vs =
 "#version 330\n"
 "float f(float f,float v)"
 "{"
   "float s=v/4.*60.,x=15.,e=.8;"
   "return smoothstep(-x/2.,-x*(1.-e)/2.,-abs(f-s-x*.5));"
 "}"
 "vec3 f(float f)"
 "{"
   "float s=f+2.;"
   "vec3 x=vec3(s,s*s,s*s*s),e=fract(x*222.)+vec3(2.);"
   "return fract(x*e.yzx*e.zxy);"
 "}"
 "float n(float f)"
 "{"
   "return f/44100;"
 "}"
 "float t(float f)"
 "{"
   "return 1.-smoothstep(0.,4.,f);"
 "}"
 "float r(float f)"
 "{"
   "float s=60.-f;"
   "return 1.-smoothstep(0.,4.,s);"
 "}"
 "out vec2 out_sample;"
 "float p(float f)"
 "{"
   "return sign(fract(f)-.5);"
 "}"
 "float s(float f)"
 "{"
   "float s=fract(f)*2.;"
   "return s<1.?s:2.-s;"
 "}"
 "float x(float f)"
 "{"
   "return-4.*f+4.*sqrt(f);"
 "}"
 "float f(float s,float x,float v,float m)"
 "{"
   "return floor(f(floor(s*x)/(x*60.)+m).x*(v+1.))/v;"
 "}"
 "float n(float f,float s)"
 "{"
   "float x=sin(6.28319*f);"
   "return pow(abs(x),s)*sign(x);"
 "}"
 "vec2 v(float f)"
 "{"
   "float s=1.,x=3.,e=s/x,v=floor(fract(e*f)*x),a=fract(f*s),g=max(1.-a*2.,0.);"
   "return vec2(g*n(f*100.+n(a,exp2(v-1.))*20.,.5));"
 "}"
 "void main()"
 "{"
   "float s=n(gl_VertexID);"
   "const float x=6.28319;"
   "float e=s*x,a=floor(s*8.),g=exp(floor(f(a/8./60.).x*8.)*.25)+160.,t=.125,p=f(s,t,3.,.01),o=f(s,t,3.,.0011),m=.5,y=floor(fract(s*m)*2.),D=m*2.*(1.+mix(p,o,y)*3.),l=fract(s*D),c=sin(l*3.14159)*.5+max(sin(l*2*3.14159),0.)*.5,u=f(s,D,8.,0.),i=exp(u+3.)*33.,h=(f(s,1.)*.2+f(s,3.))*40.,z=h*sin(l*2.)*4.,w=f(s,2.)*sin(e*i*.5);"
   "out_sample=vec2(0.);"
   "out_sample+=vec2(sin(e*i+z+w),sin(e*i-z+w))*.25*c;"
   "out_sample+=v(s)*.125+v(s*1.001)*.125*f(s,2.);"
   "out_sample*=pow(1.-r(s),2.);"
 "}";

#endif // SOUND_VS_HPP_