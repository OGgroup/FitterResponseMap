#include <VertexResponse.h>

// Vertex Response Collection
namespace frp
{
  VertexResponse::VertexResponse(double resolution_x, double resolution_y, double resolution_z) :
    resolution_x(resolution_x), resolution_y(resolution_y), resolution_z(resolution_z)
  {
    // X
    this->function_x = std::unique_ptr<TF1> (
      new TF1("ERFunctionX", "exp(-(x)^2/2/[0]^2)", -100*resolution_x, 100*resolution_x) );
    this->function_x->SetParameter(0, resolution_x);
    // Y
    this->function_y = std::unique_ptr<TF1> (
      new TF1("ERFunctionY", "exp(-(x)^2/2/[0]^2)", -100*resolution_y, 100*resolution_y) );
    this->function_y->SetParameter(0, resolution_y);
    // Z
    this->function_z = std::unique_ptr<TF1> (
      new TF1("ERFunctionZ", "exp(-(x)^2/2/[0]^2)", -100*resolution_z, 100*resolution_z) );
    this->function_z->SetParameter(0, resolution_z);
  }

  std::array<double,3> VertexResponse::GetRandomVertex(std::array<double, 3> vtx)
  {
    // Check for out of bounds
    if( (resolution_x <= 0.001) || (resolution_y <= 0.001) || (resolution_z <= 0.001) )
      return std::array<double,3>({ -99999, -99999, -99999 });

    std::array<double,3> newvtx({ this->function_x->GetRandom()+vtx[0],
                                  this->function_y->GetRandom()+vtx[1],
                                  this->function_z->GetRandom()+vtx[2] });
    return newvtx;
  }
}
