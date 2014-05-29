/* 
    This file is part of yamcr, an experimental Monte Carlo renderer

    Copyright (c) 2013-2014 by Tzu-Mao Li

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License Version 3
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "environment.h"
#include "texturesystem.h"

#include <cmath>

namespace yamcr {

EnvironmentLight::EnvironmentLight(const std::string &filename) {
    m_Filename = OpenImageIO::ustring(filename);
    m_Options.swrap = OpenImageIO::TextureOptions::Wrap::WrapPeriodic;
    m_Options.twrap = OpenImageIO::TextureOptions::Wrap::WrapPeriodic;
    m_Options.nchannels = RGBSpectrum::Dimension; // might be changed
    const OpenImageIO::ImageSpec *spec = TextureSystem::s_TextureSystem->imagespec(m_Filename, 0);
    if(spec == NULL)       
        throw std::runtime_error("Error during loading texture");
    int width = spec->width;
    int height = spec->height;
    std::vector<float> weight(width*height);
    int i = 0;
    for(int y = 0; y < height; y++) {
        float ny = ((float)y + 0.5f) / (float)height;
        float sinTheta = std::sin(M_PI * ny);
        for(int x = 0; x < width; x++) {
            float nx = ((float)x + 0.5f) / (float)width;
            RGBSpectrum result;
            if(!TextureSystem::s_TextureSystem->texture(m_Filename, m_Options, nx, ny, 
                0.f, 0.f, 0.f, 0.f, result.data.data())) {
                throw std::runtime_error("Error during texture lookup");
            }
            weight[i++] = result.Y()*sinTheta;
        }
    }
    m_Distribution = new PiecewiseConstant2D(&weight[0], width, height);
}

EnvironmentLight::~EnvironmentLight() {
    delete m_Distribution;
}

RGBSpectrum EnvironmentLight::EvalDirect(const Vector &wi) const {
    float p = std::atan2(wi[1], wi[0]);
    float s = (p < 0.f ? p + 2.f*M_PI : p) * (1.f / (2.f*M_PI));
    float t = std::acos(wi[2]) * (1.f / M_PI);
    RGBSpectrum L(0.f);
    if(!TextureSystem::s_TextureSystem->texture(m_Filename, m_Options, s, t,
                0.f, 0.f, 0.f, 0.f, L.data.data())) {
        throw std::runtime_error("Error during texture lookup");
    }
    return L;
}

RGBSpectrum EnvironmentLight::SampleDirect(const Point2 &sample,
        const Intersection &isect, Ray &ray, float *pdf) const {
    Point2 uv = m_Distribution->SampleContinuous(sample, pdf);       
    if(*pdf <= 0.f)
        return RGBSpectrum(0.f);
    ray.org = isect.p;
    float theta = uv[1] * M_PI, phi = uv[0] * 2.f * M_PI;
    float costheta = std::cos(theta), sintheta = std::sin(theta);
    float sinphi = std::sin(phi), cosphi = std::cos(phi);
    *pdf /= (2.f * M_PI * M_PI * sintheta); //jacobian
    ray.dir = Vector(sintheta * cosphi, sintheta * sinphi, costheta);
    ray.tnear = isect.rayEpsilon;
    ray.tfar = std::numeric_limits<float>::infinity();
    RGBSpectrum L(0.f);
    if(!TextureSystem::s_TextureSystem->texture(m_Filename, m_Options, uv[0], uv[1],
                0.f, 0.f, 0.f, 0.f, L.data.data())) {
        throw std::runtime_error("Error during texture lookup");
    }
    
    return L;
}

float EnvironmentLight::SampleDirectPdf(
            const Intersection &isect, const Vector &wi) const {
    float p = std::atan2(wi[1], wi[0]);
    float s = (p < 0.f ? p + 2.f*M_PI : p) * (1.f / (2.f*M_PI));
    float theta = std::acos(wi[2]);
    float t = theta * (1.f / M_PI);
    return m_Distribution->Pdf(s, t) / (2.f * M_PI * M_PI * std::sin(theta));
}

}

