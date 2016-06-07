// This file is distributed under the MIT license.
// See the LICENSE file for details.

#pragma once

#ifndef VSNRAY_SIMPLE_INL
#define VSNRAY_SIMPLE_INL 1

#include <visionaray/get_surface.h>
#include <visionaray/result_record.h>
#include <visionaray/traverse.h>


namespace visionaray
{
namespace simple
{

template <typename Params>
struct kernel
{

    Params params;

    template <typename Intersector, typename R>
    VSNRAY_FUNC result_record<typename R::scalar_type> operator()(Intersector& isect, R ray) const
    {
        using S = typename R::scalar_type;
        using V = typename result_record<S>::vec_type;
        using C = spectrum<S>;

        result_record<S> result;

        auto hit_rec = closest_hit(ray, params.prims.begin, params.prims.end, isect);

        if (any(hit_rec.hit))
        {
            hit_rec.isect_pos = ray.ori + ray.dir * hit_rec.t;

            auto surf = get_surface(hit_rec, params);
            auto ambient = surf.material.ambient() * C(from_rgba(params.ambient_color));
            auto shaded_clr = select( hit_rec.hit, ambient, C(from_rgba(params.bg_color)) );
            auto view_dir = -ray.dir;

            auto n = surf.shading_normal;

#if 1 // two-sided
            n = faceforward( n, view_dir, surf.geometric_normal );
#endif

            for (auto it = params.lights.begin; it != params.lights.end; ++it)
            {
                auto sr         = make_shade_record<Params, S>();
                sr.active       = hit_rec.hit;
                sr.isect_pos    = hit_rec.isect_pos;
                sr.normal       = n;
                sr.view_dir     = -ray.dir;
                sr.light_dir    = normalize( V(it->position()) - hit_rec.isect_pos );
                sr.light        = *it;
                auto clr        = surf.shade(sr);

                shaded_clr     += select( hit_rec.hit, clr, C(0.0) );
            }

            result.color        = select( hit_rec.hit, to_rgba(shaded_clr), params.bg_color );
            result.isect_pos    = hit_rec.isect_pos;
        }
        else
        {
            result.color = params.bg_color;
        }

        result.hit = hit_rec.hit;
        return result;
    }

    template <typename R>
    VSNRAY_FUNC result_record<typename R::scalar_type> operator()(R ray) const
    {
        default_intersector ignore;
        return (*this)(ignore, ray);
    } 
};

} // simple
} // visionaray

#endif // VSNRAY_SIMPLE_INL
