#ifndef D46AD6314A0C457CA536F466A8EAF4CD
#define D46AD6314A0C457CA536F466A8EAF4CD

#include "utilities/Definitions.hpp"

namespace nxgl::gfx
{
struct IColorable
{
  virtual nxgl::nxColor operator()( uint32_t index ) const = 0;
};

/***
 * creates a solid / uniform color
 */
class SolidColorizer : public IColorable
{
public:

  void setColor( const nxgl::nxColor& color ) { m_color = color; }
  [[nodiscard]] nxgl::nxColor getColor() const { return m_color; }

  nxgl::nxColor operator()( uint32_t index ) const override
  {
    return m_color;
  }

private:
  nxgl::nxColor m_color;
};

/***
 * creates gradients across a spectrum
 */
class SpectrumColorizer : public IColorable
{
public:

  ///
  /// \param intervals the number of colors to bounce between on the spectrum of start and end.
  /// \param startColor the start of the spectrum
  /// \param endColor the end of the spectrum
  void setColors( uint32_t intervals, // this does NOT refer to the number of vertices
                  const nxColor& startColor,
                  const nxColor& endColor )
  {
    assert( intervals > 2 );

    m_intervals = intervals;
    m_startColor = startColor;
    m_endColor = endColor;
    m_deltaColor = endColor - startColor;
  }

  void setIntervals( uint32_t intervals )
  {
    assert( intervals > 2 );
    m_intervals = intervals;
  }

  nxgl::nxColor operator()( uint32_t index ) const override
  {
    if ( index == 0 ) return m_startColor;
    if ( index == m_intervals - 1 ) return m_endColor;

    auto percentage = ( float )( index % m_intervals ) / ( float )m_intervals;

    return { m_deltaColor.r * percentage + m_startColor.r,
             m_deltaColor.g * percentage + m_startColor.g,
             m_deltaColor.b * percentage + m_startColor.b,
             m_deltaColor.a * percentage + m_startColor.a };
  }

private:

  // start and end of spectrum
  uint32_t m_intervals { 0 };
  nxColor m_deltaColor;
  nxColor m_startColor;
  nxColor m_endColor;

};

}

#endif //D46AD6314A0C457CA536F466A8EAF4CD
