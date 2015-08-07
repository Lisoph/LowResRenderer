#include "Rasterizer.hpp"

#include "util.hpp"

#include <iostream>
#include <intrin.h>

namespace LRR
{
  namespace Rendering
  {
    Rasterizer::Rasterizer(int frameWidth, int frameHeight)
      : mFrameBuffer(frameWidth, frameHeight)
    {}

    void Rasterizer::DrawScanlineBuffer(int x, int y, ScanlineBuffer const &slBuffer, Shader &shader) {
      //#pragma omp parallel for
      for(int i = 0; i < std::min(slBuffer.Height(), mFrameBuffer.Height()); ++i) {
        int destY = y + i;
        if(destY < 0 || destY >= mFrameBuffer.Height()) continue;

        auto &sl = slBuffer.Scanline(i);

        if(sl.min >= mFrameBuffer.Width() || sl.max >= mFrameBuffer.Width()) {
          //__debugbreak();
          //std::cout << "Error min=" << sl.min << ", max=" << sl.max << " on i=" << i << ", height=" << slBuffer.Height() << '\n';
          // TODO: Fix this bug!!
          continue;
        }

        for(int r = sl.min; r < sl.max; ++r) {
          int destX = x + r;

          if(destX < 0 || destX >= mFrameBuffer.Width()) continue;
          float fragX = float(destX) / (mFrameBuffer.Width() / 2.0f) - 1;
          float fragY = (mFrameBuffer.Height() - float(destY)) / (mFrameBuffer.Height() / 2.0f) - 1;
          shader.FragCoord({fragX, fragY});

          auto fragColor = shader.OnFragment().fragmentColor;
          auto red = uint32_t(Util::Clamp(fragColor(0) * 255, 0.0f, 255.0f));
          auto green = uint32_t(Util::Clamp(fragColor(1) * 255, 0.0f, 255.0f));
          auto blue = uint32_t(Util::Clamp(fragColor(2) * 255, 0.0f, 255.0f));
          mFrameBuffer.SetPixel(destX, destY, (red << 24) | (green << 16) | (blue << 8) | 0xFF);
        }
      }
    }

    void Rasterizer::FillTriangle(Vector3f const &a, Vector3f const &b, Vector3f const &c, Shader &shader) {
      shader.CurrentVertex(a);
      auto pos1 = shader.OnVertex().vertexPosition;
      shader.CurrentVertex(b);
      auto pos2 = shader.OnVertex().vertexPosition;
      shader.CurrentVertex(c);
      auto pos3 = shader.OnVertex().vertexPosition;

      // Perspective division
      static auto divider = [](Vector4f const &vertex) -> Vector4f {
        float div = (vertex(3) == 0 ? 1 : vertex(3));
        return vertex / div;
      };

      pos1 = divider(pos1);
      pos2 = divider(pos2);
      pos3 = divider(pos3);

      // Map to device coordinates
      static auto mapper = [&](Vector4f const &ndc) -> Vector2f {
        float width = float(mFrameBuffer.Width());
        float height = float(mFrameBuffer.Height());
        float x = (ndc(0) * 0.5f + 0.5f) * width;
        float y = (-ndc(1) * 0.5f + 0.5f) * height;
        return {x, y};
      };

      auto dev1 = mapper(pos1);
      auto dev2 = mapper(pos2);
      auto dev3 = mapper(pos3);

      auto slb = ScanlineBuffer::Triangle(dev1.cast<int>(), dev2.cast<int>(), dev3.cast<int>());
      DrawScanlineBuffer(0, 0, slb, shader);
    }

    void Rasterizer::ClipPolygonComponent(EigenVec<Vector4f> const &vertices, EigenVec<Vector4f> &clippedOut, unsigned componentIndex, float factor) {
      auto const *previousVertexPtr = &vertices[vertices.size() - 1];

      for(auto const &currentVertex : vertices) {
        auto currentComponent = currentVertex(componentIndex) * factor;
        bool currentInside = currentComponent <= currentVertex(3);

        auto const &previousVertex = *previousVertexPtr;
        auto previousComponent = previousVertex(componentIndex) * factor;
        bool previousInside = previousComponent <= previousVertex(3);

        if(currentInside ^ previousInside) {
          float previous = previousVertex(3) - previousComponent;
          float current = currentVertex(3) - currentComponent;
          float t = (previous) / (previous - current);
          clippedOut.push_back(Util::Lerp<Vector4f>(previousVertex, currentVertex, t));
        }

        if(currentInside) {
          clippedOut.push_back(currentVertex);
        }

        previousVertexPtr = &currentVertex;
      }
    }

    bool Rasterizer::ClipPolygonAxis(EigenVec<Vector4f> &vertices, EigenVec<Vector4f> &auxList, unsigned componentIndex) {
      ClipPolygonComponent(vertices, auxList, componentIndex, 1.0f);
      vertices.clear();

      if(auxList.empty()) {
        return false;
      }

      ClipPolygonComponent(auxList, vertices, componentIndex, -1.0f);
      auxList.clear();

      return !vertices.empty();
    }

    void Rasterizer::DrawTriangle(Vector3f const &a, Vector3f const &b, Vector3f const &c, Shader &shader) {
      EigenVec<Vector4f> vertices{3};
      vertices.push_back(Vector4f{a(0), a(1), a(2), 1});
      vertices.push_back(Vector4f{b(0), b(1), b(2), 1});
      vertices.push_back(Vector4f{c(0), c(1), c(2), 1});
      EigenVec<Vector4f> auxList{3};

      if(ClipPolygonAxis(vertices, auxList, 0) &&
         ClipPolygonAxis(vertices, auxList, 1) &&
         ClipPolygonAxis(vertices, auxList, 2)) {
        auto &initialVertex = vertices[0];

        std::cout << vertices.size() << ' ' << auxList.size() << '\n';

        for(auto iter = vertices.begin() + 1; iter < vertices.end() - 1; ++iter) {
          FillTriangle(initialVertex.head<3>(), (*iter).head<3>(), (*(iter + 1)).head<3>(), shader);
        }
      }
    }
  }
}