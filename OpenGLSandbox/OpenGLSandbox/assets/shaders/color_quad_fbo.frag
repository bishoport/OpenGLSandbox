#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D depthTexture; // El depth buffer
uniform float focusDepth;
uniform float blurRadius;

void main()
{
    float depth = texture(depthTexture, TexCoords).r;
    vec3 col = texture(screenTexture, TexCoords).rgb;
    
    float factor = abs(depth - focusDepth);
    
    if (factor < blurRadius) 
    {
        FragColor = vec4(col, 1.0);
    } 
    else 
    {
        // Aplica desenfoque en función de la diferencia de profundidad
        vec3 blurredCol = vec3(0.0);
        int samples = 10;
        
        float blurSize = 0.005 * factor; // Aumentar el tamaño del desenfoque con la profundidad
        
        for (int i = -samples / 2; i <= samples / 2; ++i) 
        {
            for (int j = -samples / 2; j <= samples / 2; ++j) 
            {
                vec2 sampleCoords = TexCoords + vec2(i, j) * blurSize;
                blurredCol += texture(screenTexture, sampleCoords).rgb;
            }
        }
        blurredCol /= float(samples * samples);
        FragColor = vec4(blurredCol, 1.0);
    }
}




//#version 460 core
//out vec4 FragColor;
//
//in vec2 TexCoords;
//
//uniform sampler2D screenTexture;
//
//void main()
//{
//    // Obtener el color original de la textura
//    vec3 col = texture(screenTexture, TexCoords).rgb;
//    
//    // Invertir cada componente de color
//    vec3 invertedCol = 1.0 - col;
//    
//    // Asignar el color invertido a FragColor con alfa de 1.0
//    FragColor = vec4(invertedCol, 1.0);
//}



//#version 460 core
//out vec4 FragColor;
//
//in vec2 TexCoords;
//
//uniform sampler2D screenTexture;
//
//void main()
//{
//    vec3 col = texture(screenTexture, TexCoords).rgb;
//    FragColor = vec4(col, 1.0);
//}