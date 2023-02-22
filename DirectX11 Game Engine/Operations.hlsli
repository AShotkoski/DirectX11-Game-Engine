
float Attenuate(float d_px_light, float quad, float lin, float cons)
{
    return 1.0f
                                    /
         (d_px_light * d_px_light * quad + d_px_light * lin + cons);
}

float3 Diffuse(float3 color, float intensity, float3 n_px_to_light, float3 n_normal)
{
    return max(0, dot(n_px_to_light, n_normal)) * color * intensity;
}

float3 Speculate_BlinnPhong
(float3 color, float power, float3 n_px_to_eye, float3 n_px_to_light, float3 n_normal)
{
    if(power == 0)
        power = 1;
    const float3 n_halfWay = normalize(n_px_to_eye + n_px_to_light);
    float cosBetween = max(0, dot(n_normal, n_halfWay));
    return color * pow(cosBetween, power);
}