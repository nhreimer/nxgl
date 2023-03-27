#ifndef E62CD1E9AB264B5FAE62A60AB06AC166
#define E62CD1E9AB264B5FAE62A60AB06AC166

namespace nxgl::gfx
{

template < typename TData >
struct IGenerator
{
  [[nodiscard]] virtual GLenum getMode() const = 0;
  virtual std::vector< TData > generateVertices( uint8_t edges ) = 0;
  virtual std::vector< uint8_t > generateIndices( uint8_t edges ) = 0;
};

/// ABC, 012
/// ACD, 023
/// ADE, 034
/// \tparam TData
template < typename TData >
class CCWABCPolygon : public IGenerator< TData >
{
public:

  [[nodiscard]] GLenum getMode() const override { return GL_TRIANGLE_FAN; }

  std::vector< TData > generateVertices( uint8_t edges ) override
  {
    std::vector< TData > vertexBuffer( edges + 1 );

    // divide a circle by the number of edges
    // which is A(0, 0) -> B(0, 1) -> C(1, 1)
    // note that C is slightly less than 1, 1 however
    auto angle = nxgl::NX_TAU / ( float )edges;

    vertexBuffer[ 0 ] = TData::createData( { 0.f, 0.f } );
    vertexBuffer[ 1 ] = TData::createData( { std::cos( 0 ), std::sin( 0 ) } );

    for ( int edge = 1, posInBuffer = 2; edge < edges; ++edge, ++posInBuffer )
    {
      auto vertexAngle = ( float )edge * angle;
      vertexBuffer[ posInBuffer ] = TData::createData( { std::cos( vertexAngle ), std::sin( vertexAngle ) } );
    }

    return std::move( vertexBuffer );
  }

  std::vector< uint8_t > generateIndices( uint8_t edges ) override
  {
    std::vector< uint8_t > indexBuffer( edges * 3 );

    for ( int idx = 0, edge = 0; edge < edges; idx += 3, ++edge )
    {
      indexBuffer[ idx ] = 0;
      indexBuffer[ idx + 1 ] = edge + 1;
      if ( edge + 1 < edges )
        indexBuffer[ idx + 2 ] = edge + 2;
      else
        indexBuffer[ idx + 2 ] = 1;
    }

    return std::move( indexBuffer );
  }

};

/// CBA, 210
/// DCA, 320
/// EDA, 430
/// \tparam TData
template < typename TData >
class CCWCBAPolygon : public CCWABCPolygon< TData >
{
public:

  [[nodiscard]] GLenum getMode() const override { return GL_TRIANGLE_FAN; }

  std::vector< uint8_t > generateIndices( uint8_t edges ) override
  {
    std::vector< uint8_t > indexBuffer( edges * 3 );

    for ( int idx = 0, edge = 0; edge < edges; idx += 3, ++edge )
    {
      if ( edge + 1 < edges )
        indexBuffer[ idx ] = edge + 2;
      else
        indexBuffer[ idx ] = 1;

      indexBuffer[ idx + 1 ] = edge + 1;
      indexBuffer[ idx + 2 ] = 0;
    }

    return std::move( indexBuffer );
  }

};

/// BCA, 120
/// CDA, 230
/// DDA, 340
/// \tparam TData
template < typename TData >
class CCWBCAPolygon : public CCWABCPolygon< TData >
{
public:

  [[nodiscard]] GLenum getMode() const override { return GL_TRIANGLE_FAN; }

  std::vector< uint8_t > generateIndices( uint8_t edges ) override
  {
    std::vector< uint8_t > indexBuffer( edges * 3 );

    for ( int idx = 0, edge = 0; edge < edges; idx += 3, ++edge )
    {
      indexBuffer[ idx ] = edge + 1;
      if ( edge + 1 < edges )
        indexBuffer[ idx + 1 ] = edge + 2;
      else
        indexBuffer[ idx + 1 ] = 1;
      indexBuffer[ idx + 2 ] = 0;
    }

    return std::move( indexBuffer );
  }

};

}

#endif //E62CD1E9AB264B5FAE62A60AB06AC166
