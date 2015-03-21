/* File generated with Shader Minifier 1.1.4
 * http://www.ctrl-alt-test.fr
 */
#ifndef RAYMARCHING_FS_HPP_
# define RAYMARCHING_FS_HPP_

const char *raymarching_fs =
 "#version 420\n"
 "float s(float v,float m)"
 "{"
   "float s=m/4.*60.,f=15.,c=.8;"
   "return smoothstep(-f/2.,-f*(1.-c)/2.,-abs(v-s-f*.5));"
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
 "mat3 f(vec3 v,vec3 m)"
 "{"
   "vec3 f=normalize(v-m),c=normalize(cross(vec3(0.,1.,0.),f)),s=normalize(cross(f,c));"
   "mat3 r=mat3(c,s,f);"
   "return r;"
 "}"
 "uniform bool is_shadowmap;"
 "const mat4 m=mat4(1./1.5,0.,0.,0.,0.,1./1.5,0.,0.,0.,0.,-.4,0.,0.,0.,-1.4,1.);"
 "vec3 f()"
 "{"
   "return vec3(1.5,3.,2.);"
 "}"
 "mat3 n()"
 "{"
   "return f(f(),vec3(0.));"
 "}"
 "vec3 x(float v)"
 "{"
   "return is_shadowmap?f():vec3(.3,2.+cos(v),3.+sin(v)*.8);"
 "}"
 "mat3 n(float m,vec3 v)"
 "{"
   "return is_shadowmap?n():f(v,vec3(0.));"
 "}"
 "vec3 l(float v)"
 "{"
   "float m=max(s(v,1.),s(v,3.));"
   "return vec3(sin(6.28319*v*.5)*.2*m,abs(sin(6.28319*v*.5))*.6*m,0.);"
 "}"
 "float l(vec3 v,float m)"
 "{"
   "vec3 f=pow(abs(v),vec3(m));"
   "return pow(f.x+f.y+f.z,1/m);"
 "}"
 "float v(vec3 v,float m)"
 "{"
   "return length(v)-m;"
 "}"
 "float x(vec3 v,float m)"
 "{"
   "return length(v.yz)-m;"
 "}"
 "float a(vec3 v,float m)"
 "{"
   "return length(v.xz)-m;"
 "}"
 "vec3 t(vec3 v,float m)"
 "{"
   "return v-l(m);"
 "}"
 "vec3 i(vec3 v,float m)"
 "{"
   "vec3 f=t(v,m);"
   "return vec3(abs(f.x),f.yz);"
 "}"
 "float e(vec3 v,float m)"
 "{"
   "if(is_shadowmap)"
     "return 99999.9;"
   "vec3 r=i(v,m);"
   "r-=vec3(.2,.2,.42);"
   "vec3 f=mat3(cos(.3)*cos(-.4),-sin(.3)*cos(-.4),-sin(-.4),sin(.3),cos(.3),0.,cos(.3)*sin(-.4),-sin(.3)*sin(-.4),cos(-.4))*r;"
   "return max(max(-f.x,f.x-.15),x(f,.014));"
 "}"
 "float c(vec3 m,float f)"
 "{"
   "if(is_shadowmap)"
     "return 99999.9;"
   "vec3 s=i(m,f);"
   "return min(e(m,f),v(s-vec3(.23,.04,.4),.09));"
 "}"
 "float p(vec3 m,float f)"
 "{"
   "vec3 c=t(m,f),r=i(m,f);"
   "float p=abs(sin(f*3.)*3.14159*.5*s(f,1));"
   "vec3 l=mat3(1.,0.,0.,0.,cos(p),sin(p),0.,-sin(p),cos(p))*(c-vec3(0.,cos(acos(.9))*.5,sin(-acos(.9))*.5))+vec3(0.,cos(acos(.9))*.5,sin(-acos(.9))*.5);"
   "float h=min(min(max(v(c,.5),c.y-cos(acos(.9))*.5),max(a(c,.15),max(c.y-cos(acos(.9))*.5-.03,-c.y+cos(acos(.9))*.5))),max(max(v(l,.5),-(l.y-cos(acos(.9))*.5)),-max(a(l,.15),max(l.y-cos(acos(.9))*.5-.03,-l.y+cos(acos(.9))*.5-.1))));"
   "return min(h,v(r-vec3(.045,-.1,.49),.06));"
 "}"
 "float h(vec3 m,float v)"
 "{"
   "vec3 f=m-vec3(0.,-.5,0.),c=fract(f)*2.-1.;"
   "return max(l(c,8.)-1.05,f.y);"
 "}"
 "float r(vec3 v,float m)"
 "{"
   "return min(min(p(v,m),c(v,m)),h(v,m));"
 "}struct material{vec3 diffuse;vec3 ambient;};"
 "material d(vec3 v,float m)"
 "{"
   "float f=h(v,m),s=p(v,m),a=c(v,m);"
   "if(s<f&&s<a)"
     "return material(vec3(.9,.8,.3),vec3(.3,.25,.1));"
   "else"
     " if(a<f)"
       "return material(vec3(.03),vec3(.005));"
     "else"
       " return material(vec3(.1,.8,.1),vec3(.23,.23,.3));"
 "}"
 "vec3 w(vec3 v,float m)"
 "{"
   "const float f=.0001;"
   "return normalize(vec3(r(v+vec3(f,0.,0.),m)-r(v+vec3(-f,0.,0.),m),r(v+vec3(0.,f,0.),m)-r(v+vec3(0.,-f,0.),m),r(v+vec3(0.,0.,f),m)-r(v+vec3(0.,0.,-f),m)));"
 "}"
 "uniform float sample_count;"
 "layout(binding=1)uniform sampler2DShadow shadowmap;"
 "in vec3 vary_ray_dir;"
 "out vec3 color;"
 "void main()"
 "{"
   "float c=n(sample_count);"
   "vec3 s=is_shadowmap?vary_ray_dir:x(c);"
   "mat3 i=n(c,x(c));"
   "vec3 a=is_shadowmap?-i[2]:normalize(vary_ray_dir);"
   "float h=is_shadowmap?0.:length(vary_ray_dir);"
   "vec3 p=i[2];"
   "color=vec3(-a.x,-a.y,a.z);"
   "gl_FragDepth=.9999;"
   "vec3 l=vec3(0.),z=vec3(0.);"
   "material t=material(vec3(0.),vec3(0.));"
   "for(int e=0;e<128;++e)"
     "{"
       "l=a*h+s;"
       "float g=r(l,c);"
       "if(g<.001)"
         "{"
           "float y=h*dot(a,p);"
           "if(is_shadowmap)"
             "{"
               "gl_FragDepth=clamp(-y/5.,0.,.9999);"
               "return;"
             "}"
           "else"
             " gl_FragDepth=clamp((6.1+1.2/y)/5.9*.5+.5,0.,.9999);"
           "z=w(l,c);"
           "t=d(l,c);"
           "break;"
         "}"
       "h+=g;"
     "}"
   "if(is_shadowmap)"
     "return;"
   "mat3 g=n();"
   "vec3 e=g[2];"
   "float y=dot(e,z);"
   "vec3 b=max(y,0.)*t.diffuse;"
   "if(y>0.)"
     "{"
       "mat3 o=transpose(g);"
       "vec4 u=m*vec4(o*(l-f()),1.);"
       "u.xyz*=.5;"
       "u.xyz+=.5;"
       "u.z-=.01;"
       "float D=textureProj(shadowmap,u);"
       "color=D*b+t.ambient;"
     "}"
   "else"
     " color=t.ambient;"
   "color*=1.-max(v(c),f(c));"
 "}";

#endif // RAYMARCHING_FS_HPP_
