#include "Rasterizer.h"

void Rasterizer::StartUp() noexcept
{
	RendererBase::StartUp();
}

void Rasterizer::Draw() noexcept
{
	// do nothing now
	Sleep(10);
}

Rasterizer::~Rasterizer()
{
	RendererBase::~RendererBase();
}

//std::vector<std::vector<float>> triangleTraversal(TGAImage* frameBuffer, std::vector<std::vector<float>>&& v_out) {
//    std::vector<std::vector<float>> f_in;
//
//    // 生成三角形的二维包围盒
//    Vec2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
//    Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
//    Vec2f clamp(frameBuffer->get_width() - 1, frameBuffer->get_height() - 1);
//    Vec3f pts[3];
//    int n_faces = v_out.size() / 3;
//
//    for (int i = 0; i < n_faces; i++)
//    {
//        for (int j = 0; j < 3; j++)
//        {
//            pts[j] = Vec3f(v_out[i * 3 + j][0], v_out[i * 3 + j][1], v_out[i * 3 + j][2]);
//            for (int k = 0; k < 2; k++)
//            {
//                bboxmin[k] = std::max(0.0f, std::min(bboxmin[k], pts[j][k]));
//                bboxmax[k] = std::min(clamp[k], std::max(bboxmax[k], pts[j][k]));
//            }
//        }
//
//        // 光栅化
//        Vec3f P;
//        for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++)
//        {
//            for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++)
//            {
//                // 得到包围盒内像素的重心坐标判断是否在三角形内
//                Vec3f bc_screen = barycentric(pts[0], pts[1], pts[2], P);
//                if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0)
//                {
//                    continue;
//                }
//
//                // 插值目标像素的z值
//                P.z = 0;
//                for (int j = 0; j < 3; j++) P.z += pts[j][2] * bc_screen[j];
//
//                // 插值模视变换之前的片元顶点位置传递到后面
//                Vec3f ori_P;
//                for (int j = 0; j < 3; j++)
//                {
//                    ori_P.x += v_out[i * 3 + j][3] * bc_screen[j];
//                    ori_P.y += v_out[i * 3 + j][4] * bc_screen[j];
//                    ori_P.z += v_out[i * 3 + j][5] * bc_screen[j];
//                }
//                std::vector<float> fragment = { float(i), float(P.x), float(P.y), float(P.z), bc_screen.x, bc_screen.y, bc_screen.z ,ori_P.x, ori_P.y ,ori_P.z };
//                f_in.push_back(fragment);
//            }
//        }
//    }
//
//    return f_in;
//}
//
//std::vector<std::vector<float>> vertexShading(Model* model, v_Shader* v_shader) {
//    std::vector<std::vector<float>> v_out;
//
//    for (int i = 0; i < model->nfaces(); i++)
//    {
//        for (int j = 0; j < 3; j++)
//        {
//            v_out.push_back(v_shader->vertex(i, model->face(i)[j * 2]));
//        }
//    }
//
//    return v_out;
//}
//
//std::vector<std::vector<float>> fragmentShading(std::vector<std::vector<float>>&& f_in, f_Shader* f_shader) {
//    std::vector<std::vector<float>> f_out;
//
//    int n_fragments = f_in.size();
//
//    for (int i = 0; i < n_fragments; i++)
//    {
//        f_out.push_back(f_shader->fragment(f_in[i]));
//    }
//
//    return f_out;
//}
//
//float** depthTestAndBlend(TGAImage* frameBuffer, std::vector<std::vector<float>> f_out) {
//    float** zBuffer = makeZBuffer(frameBuffer->get_height(), frameBuffer->get_width());
//    int n_fragments = f_out.size();
//
//    for (int i = 0; i < n_fragments; i++)
//    {
//        std::vector<float> f = f_out[i];
//
//        int x = f[0];
//        int y = f[1];
//        float z = f[2];
//
//        if (zBuffer[x][y] < z)
//        {
//            // BGRA
//            frameBuffer->set(x, y, TGAColor(f[5], f[4], f[3], f[6]));
//            zBuffer[x][y] = z;
//        }
//    }
//
//    return zBuffer;
//}
//
//RenderArgs* makeShadowArgs(Model* model) {
//    int shadow_width = width;
//    int shadow_height = height;
//
//    TGAImage* shadow_buffer = new TGAImage(shadow_width, shadow_height, TGAImage::RGB);
//    modelViewMat = makeViewMat(light_dir, center, Vec3f(0, 1, 0));
//    viewportMat = makeViewportMat(shadow_width / 8, shadow_height / 8, shadow_width * 3 / 4, shadow_height * 3 / 4);
//    projMat = makeProjMat(0);
//
//    Matrix* all_mat_vertex = new Matrix(viewportMat * projMat * modelViewMat);
//
//    v_Basic* v_shader = new v_Basic();
//    v_shader->vpm_mat = all_mat_vertex;
//    v_shader->model = model;
//
//    f_Depth* f_shader = new f_Depth();
//
//    RenderArgs* shadowArgs = new RenderArgs(shadow_buffer, model, v_shader, f_shader);
//    shadowArgs->mat = all_mat_vertex;
//    return shadowArgs;
//}
//
//RenderArgs* makeMainArgs(Model* model, TGAImage* diffuse, TGAImage* specular, TGAImage* normalMap, TGAImage* AOMap, RenderArgs* shadowArgs) {
//    TGAImage* frameBuffer = new TGAImage(width, height, TGAImage::RGB);
//
//    modelViewMat = makeViewMat(eye, center, Vec3f(0, 1, 0));
//    viewportMat = makeViewportMat(width / 8, height / 8, width * 3 / 4, height * 3 / 4);
//    projMat = makeProjMat((eye - center).norm());
//
//    Matrix* all_mat_vertex = new Matrix(viewportMat * projMat * modelViewMat);
//    Matrix all_mat = projMat * modelViewMat;
//    Matrix* all_mat_invert_transpose = new Matrix(all_mat.invert_transpose());
//    Vec4f lightDir_4 = all_mat * Vec4f(light_dir.x, light_dir.y, light_dir.z, 0.0f);
//    light_dir = Vec3f(lightDir_4.x, lightDir_4.y, lightDir_4.z);
//
//    v_Basic* v_shader = new v_Basic();
//    v_shader->vpm_mat = all_mat_vertex;
//    v_shader->model = model;
//
//    f_Phong* f_shader = new f_Phong();
//    f_shader->mat_invert_transpose = all_mat_invert_transpose;
//    f_shader->lightDir = new Vec3f(light_dir.normalize());
//    f_shader->viewDir = new Vec3f((eye - center).normalize());
//    f_shader->model = model;
//    f_shader->diffuse = diffuse;
//    f_shader->specular = specular;
//    f_shader->normalMap = normalMap;
//    f_shader->shadowMap = shadowArgs->frameBuffer;
//    f_shader->shadowMat = shadowArgs->mat;
//    f_shader->ambientMap = AOMap;
//
//    //f_SSAO* f_shader = new f_SSAO();
//
//    RenderArgs* mainArgs = new RenderArgs(frameBuffer, model, v_shader, f_shader);
//    return mainArgs;
//}
//
//float** render(RenderArgs* args) {
//    std::vector<std::vector<float>> v_out = std::move(vertexShading(args->model, args->v_shader));
//    std::vector<std::vector<float>> f_in = std::move(triangleTraversal(args->frameBuffer, std::move(v_out)));
//    std::vector<std::vector<float>> f_out = std::move(fragmentShading(std::move(f_in), args->f_shader));
//    return depthTestAndBlend(args->frameBuffer, f_out);
//}
//
//void SSAO(RenderArgs* mainArgs, float** zBuffer) {
//    int h = mainArgs->frameBuffer->get_height();
//    int w = mainArgs->frameBuffer->get_width();
//    for (int y = 0; y < h; y++)
//    {
//        for (int x = 0; x < w; x++)
//        {
//            if (zBuffer[y][x] < 1)
//            {
//                continue;
//            }
//            float total = 0;
//            // 朝当前点周围方向发射射线, 附带随机扰动
//            for (float a = 0; a < M_PI * 2 - 1e-4; a += M_PI / 4)
//            {
//                srand(time(NULL));
//                float bias = 0.1 * ((float)rand() / (float)RAND_MAX);
//                total += (M_PI / 2) - max_elevation_angle(zBuffer, Vec2f(x, y), Vec2f(cos(a + bias), sin(a + bias)));
//            }
//            total /= (M_PI / 2) * 8;
//            //total = pow(total, 10);
//            TGAColor color = mainArgs->frameBuffer->get(y, x) * (total);
//            mainArgs->frameBuffer->set(y, x, color);
//        }
//    }
//}
//
//void multipassRender(Model* model, TGAImage* diffuse = nullptr, TGAImage* specular = nullptr, TGAImage* normalMap = nullptr, TGAImage* AOMap = nullptr) {
//    RenderArgs* shadowArgs = makeShadowArgs(model);
//    float** shadowZBuffer = render(shadowArgs);
//    deleteZBuffer(shadowZBuffer, height);
//
//    RenderArgs* mainArgs = makeMainArgs(model, diffuse, specular, normalMap, AOMap, shadowArgs);
//    float** zBuffer = render(mainArgs);
//
//    SSAO(mainArgs, zBuffer);
//
//    mainArgs->frameBuffer->flip_vertically();
//    mainArgs->frameBuffer->write_tga_file("framebuffer.tga");
//
//    deleteZBuffer(zBuffer, height);
//    delete shadowArgs;
//    shadowArgs = nullptr;
//    delete mainArgs;
//    mainArgs = nullptr;
//}
//
//void makeAOMap(Model* model, TGAImage* diffuse, int sampleTimes = 5000) {
//    int shadow_width = width / 1;
//    int shadow_height = height / 1;
//
//    TGAImage* AOBuffer = new TGAImage(diffuse->get_width(), diffuse->get_height(), TGAImage::RGB);
//    TGAImage* shadow_buffer = new TGAImage(shadow_width, shadow_height, TGAImage::RGB);
//
//    v_Basic* v_shader = new v_Basic();
//    v_shader->model = model;
//
//    f_Depth* f_depth = new f_Depth();
//    RenderArgs* args = new RenderArgs(shadow_buffer, model, v_shader, f_depth);
//
//    f_AO* f_ao = new f_AO();
//    f_ao->model = model;
//    f_ao->shadowMap = shadow_buffer;
//    f_ao->AOMap = new TGAImage(diffuse->get_width(), diffuse->get_height(), TGAImage::RGB);
//
//    projMat = makeProjMat(0);
//    viewportMat = makeViewportMat(shadow_width / 8, shadow_height / 8, shadow_width * 3 / 4, shadow_height * 3 / 4);
//
//    for (int i = 1; i <= sampleTimes; i++)
//    {
//        std::cout << "Sampling: " << i << "/" << sampleTimes << std::endl;
//        eye = rand_point_on_unit_sphere();
//        eye.y = std::abs(eye.y);
//        eye.normalize();
//        eye = eye * 2;
//        shadow_buffer->clear();
//        modelViewMat = makeViewMat(eye, center, Vec3f(0, 1, 0));
//        Matrix* all_mat_vertex = new Matrix(viewportMat * projMat * modelViewMat);
//        v_shader->vpm_mat = all_mat_vertex;
//        args->mat = all_mat_vertex;
//        args->f_shader = f_depth;
//
//        render(args);
//
//        args->f_shader = f_ao;
//        f_ao->AOMap->clear();
//
//        render(args);
//
//        for (int y = 0; y < diffuse->get_height(); y++)
//        {
//            for (int x = 0; x < diffuse->get_width(); x++)
//            {
//                float old = AOBuffer->get(y, x)[0];
//                float now = f_ao->AOMap->get(y, x)[0];
//                float color = (old * (i - 1) + now) / float(i) + 0.5f;
//                AOBuffer->set(y, x, TGAColor(color, color, color, 1));
//            }
//        }
//    }
//    delete args;
//    args = nullptr;
//    AOBuffer->flip_vertically();
//    AOBuffer->write_tga_file("AO.tga");
//}
//
//int main(int argc, char** argv) {
//    TGAImage* diffuse = new TGAImage();
//    TGAImage* specular = new TGAImage();
//    TGAImage* normalMap = new TGAImage();
//    TGAImage* AOMap = new TGAImage();
//    bool tex_success = diffuse->read_tga_file((PATH::RESOURCES + "african_head_diffuse.tga").c_str());
//    assert(tex_success);
//    tex_success = specular->read_tga_file((PATH::RESOURCES + "african_head_spec.tga").c_str());
//    assert(tex_success);
//    tex_success = normalMap->read_tga_file((PATH::RESOURCES + "african_head_nm_tangent.tga").c_str());
//    assert(tex_success);
//    tex_success = AOMap->read_tga_file((PATH::RESOURCES + "african_head_ao.tga").c_str());
//    assert(tex_success);
//    diffuse->flip_vertically();
//    specular->flip_vertically();
//    normalMap->flip_vertically();
//    AOMap->flip_vertically();
//
//    Model* model = new Model((PATH::RESOURCES + "african_head.obj").c_str());
//    model->initTextureCoord(*diffuse);
//
//    //makeAOMap(model, diffuse);
//    multipassRender(model, diffuse, specular, normalMap);
//
//    delete diffuse;
//    delete specular;
//    delete normalMap;
//    delete model;
//    return 0;
//}