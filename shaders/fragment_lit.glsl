// fragment_lit.glsl
uniform sampler2D diffuse;
uniform vec2 lightPosition;
uniform int playerDamaged;
uniform  int lightsOn;

varying vec2 texCoordVar;
varying vec2 varPosition;

float attenuate(float dist, float a, float b)
{
     return 1.0 / (1.0 + (a * dist) + (b * dist  * dist));
}

void main()
{
    


    if(lightsOn == 1){
        float brightness = attenuate(distance(lightPosition, varPosition), 1.0, 0.0);
        vec4 color = texture2D(diffuse, texCoordVar);

        if(playerDamaged == 1){
            gl_FragColor = vec4(color.r,0,0, color.a);
        }else{
            gl_FragColor = vec4(color.rgb * brightness, color.a);
        }
        //gl_FragColor = vec4(color.rgb * brightness, color.a);
        
   
    } else {
        if(playerDamaged == 1){
            vec4 color = texture2D(diffuse, texCoordVar);
            gl_FragColor = vec4(color.r,0,0, color.a);
        }else{
            gl_FragColor = texture2D(diffuse, texCoordVar);
        }
  
    }
    
}
