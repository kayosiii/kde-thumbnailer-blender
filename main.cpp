#include <iostream>
#include "blendercreator.h"
#include <QImage>

int main(int argc, char **argv) {
//   QImage img(128,128, QImage::Format_RGB32);
  QImage img;
  const QString fn("../test.blend");
  const QString fname("../test_z.blend");
  BlendCreator blender_creator;
  blender_creator.create(fn,128,128,img);
//   blender_creator.create(fname,256,256,img);
  QFile out_file("test.png");
  out_file.open(QIODevice::WriteOnly);
  img.save(&out_file,"PNG");
  out_file.close();
    return 0;
}
