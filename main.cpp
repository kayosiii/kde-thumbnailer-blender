#include <iostream>
#include "blendercreator.h"
#include <QImage>
#include <QFile>

int main(int argc, char **argv) {
  QImage img;
  const QString fn("../test.blend");
  const QString fn_z("../test_z.blend");
  BlendCreator blender_creator;
  
  blender_creator.create(fn,128,128,img);
  QFile out_file("test.png");
  out_file.open(QIODevice::WriteOnly);
  img.save(&out_file,"PNG");
  out_file.close();
  
  blender_creator.create(fn_z,128,128,img);
  QFile out_file_z("test_z.png");
  out_file_z.open(QIODevice::WriteOnly);
  img.save(&out_file_z,"PNG");
  out_file_z.close();
  
  return 0;
}
