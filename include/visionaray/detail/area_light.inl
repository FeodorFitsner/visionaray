// This file is distributed under the MIT license.
// See the LICENSE file for details.

namespace visionaray
{

//-------------------------------------------------------------------------------------------------
// area_light members
//

template <typename Geometry>
inline area_light<Geometry>::area_light(Geometry geometry)
    : geometry_(geometry)
{
}

template <typename Geometry>
template <typename T>
VSNRAY_FUNC
inline vector<3, T> area_light<Geometry>::intensity(vector<3, T> const& pos) const
{
}

template <typename Geometry>
template <typename Sampler>
VSNRAY_FUNC
inline vector<3, typename Sampler::value_type> area_light<Geometry>::sample(Sampler& samp) const
{
    return sample(geometry_, samp);
}

template <typename T>
template <size_t N, typename Sampler>
VSNRAY_FUNC
inline void area_light<T>::sample(
        array<vector<3, typename Sampler::value_type>, N>& result,
        Sampler& samp
        ) const
{
    for (size_t i = 0; i < N; ++i)
    {
        result[i] = sample(samp);
    }
}

} // visionaray
