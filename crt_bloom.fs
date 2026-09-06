#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 finalColor;

uniform sampler2D texture0;
uniform vec2 resolution;
uniform float time;

vec3 SampleBloom(vec2 uv, vec2 texelSize)
{
    vec3 bloom = vec3(0.0);

    // Slight blur around bright pixels.
    bloom += texture(texture0, uv + texelSize * vec2(-4.0, -4.0)).rgb;
    bloom += texture(texture0, uv + texelSize * vec2( 0.0, -4.0)).rgb;
    bloom += texture(texture0, uv + texelSize * vec2( 4.0, -4.0)).rgb;

    bloom += texture(texture0, uv + texelSize * vec2(-4.0,  0.0)).rgb;
    bloom += texture(texture0, uv).rgb;
    bloom += texture(texture0, uv + texelSize * vec2( 4.0,  0.0)).rgb;

    bloom += texture(texture0, uv + texelSize * vec2(-4.0,  4.0)).rgb;
    bloom += texture(texture0, uv + texelSize * vec2( 0.0,  4.0)).rgb;
    bloom += texture(texture0, uv + texelSize * vec2( 4.0,  4.0)).rgb;

    bloom /= 9.0;

    // Only allow bright parts of the image to contribute strongly.
    float brightness = max(max(bloom.r, bloom.g), bloom.b);
    float threshold = smoothstep(0.45, 1.0, brightness);

    return bloom * threshold;
}

void main()
{
    vec2 uv = fragTexCoord;

    vec2 center = uv - vec2(0.5);
    float distanceFromCenter = length(center);

    // Very slight CRT barrel distortion.
    float distortion = 0.035;

    uv += center * distanceFromCenter * distanceFromCenter * distortion;

    // Outside the screen after distortion.
    if (uv.x < 0.0 || uv.x > 1.0 ||
        uv.y < 0.0 || uv.y > 1.0)
    {
        finalColor = vec4(0.0);
        return;
    }

    vec2 texelSize = 1.0 / resolution;

    vec3 color = texture(texture0, uv).rgb;

    // Bloom.
    vec3 bloom = SampleBloom(uv, texelSize);
    color += bloom * 0.45;

    // CRT scanlines.
    float scanline =
        0.96 +
        0.04 * sin(uv.y * resolution.y * 1.2);

    color *= scanline;

    // Very subtle horizontal CRT noise.
    float noise =
        sin(uv.y * resolution.y * 0.25 + time * 20.0) *
        0.005;

    color += noise;

    // Vignette.
    float vignette =
        smoothstep(0.95, 0.30, distanceFromCenter);

    color *= vignette;

    // Slight CRT color separation.
    float chromatic = 0.0015;

    float red = texture(
        texture0,
        uv + vec2(chromatic, 0.0)
    ).r;

    float green = color.g;

    float blue = texture(
        texture0,
        uv - vec2(chromatic, 0.0)
    ).b;

    color = vec3(red, green, blue);

    finalColor = vec4(color, 1.0);
}
