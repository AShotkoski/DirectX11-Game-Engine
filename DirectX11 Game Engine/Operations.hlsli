
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

float3 CalculateNormalFromMap(float3 n_sampled, float3 n_normal, float3 n_tangent, float3 n_bitangent)
{
    // Uncompress data from map into range [-1, 1] from [0, 1]
    float3 adjusted_sampled = 2.0f * n_sampled - 1.0f; 
    // Create matrix with mostly orthagonal unit vectors to "rotate" from tangent space into object space
    float3x3 tan_to_obj = float3x3(n_tangent, n_bitangent, n_normal);
    // Convert sampled normal from tangent space into object space 
    return normalize(mul(adjusted_sampled, tan_to_obj));
}

float3 CalculateNormalFromMap_HD(float3 n_sampled, float3 n_normal, float3 n_tangent)
{
    // Uncompress data from map into range [-1, 1] from [0, 1]
    float3 adjusted_sampled = 2.0f * n_sampled - 1.0f; 
    
    // Adjust tangent to ensure it is orthagonal, and calculate bitangent on the fly with cross product
    float3 N = n_normal;
    float3 T = normalize(n_tangent - dot(n_tangent, N) * N);
    float3 B = cross(N, T);
    
    // Create matrix with orthagonal unit vectors to "rotate" from tangent space into object space
    float3x3 tan_to_obj = float3x3(T, B, N);
    // Convert sampled normal from tangent space into object space 
    return normalize(mul(adjusted_sampled, tan_to_obj));
}