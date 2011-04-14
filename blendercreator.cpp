#include "blendercreator.h"
#include <QFile>
#include <qdatastream.h>
#include <QImage>
#include <assert.h>
#include <zlib.h>
#include <kdebug.h>
#include <stdio.h>
#include <qbuffer.h>
#include <KFilterDev>


extern "C" {
  KDE_EXPORT ThumbCreator *new_creator() {
    return new BlendCreator;
  }

}


BlendCreator::BlendCreator() {}



bool BlendCreator::create(const QString& path, int width , int height, QImage& img){

  const qint32 REND = 1145980242; // "REND" as signed 32 bit number
  const qint32 TEST = 1414743380; //"TEST" as signed 32 bit number
  QFile in_file (path);

  if(!in_file.open(QIODevice::ReadOnly)) {
    kDebug(0) << "blend.open" << path << endl;
    return false;
  }
  QIODevice * gz_file;
  QDataStream in_data;
  char zipmagick[2];
  qint64 z = in_file.peek(zipmagick,2);
    unsigned char zm2 = zipmagick[1];

  if(z && zipmagick[0]==0x1f && zm2 == 0x8b) {
//     gz_file = KFilterDev::device(&in_file,"application/x-gzip",true);
//     in_file.close();
//     gz_file->open(QIODevice::ReadOnly);
//     in_data.setDevice(gz_file);
    return false;
  } else {
    in_data.setDevice(&in_file);
  }
  char magic[7];
  in_data.readRawData(magic,7);
  QByteArray magick(magic);
  if( !magick.startsWith("BLENDER")){
//     !strcmp(magic,"BLENDER",7)){
    kDebug(0) << "not a blend file " << path << endl;
    in_file.close();
    return false;
  }
  qint8 is_64_bit = 0;
  in_data >> is_64_bit;
  int header_size = (is_64_bit == '-')? 24 : 20;
  int header_remaining = header_size-8; // what is left after we remove the first two ints
  qint8 is_big_endian =0;
  in_data >> is_big_endian;
  is_big_endian = (is_big_endian == 'V')? true: false;
  char v[3];
  in_data.readRawData(v,3);
  QByteArray vers(v);
  qint32 version_ = vers.toInt();
  if(version_ < 250) {
//      kDebug(0) << "old blend file no thumbnail" << path << endl;
//     in_file.close();
//     return false;
  }
  if(!is_big_endian) in_data.setByteOrder(QDataStream::LittleEndian);
  else in_data.setByteOrder(QDataStream::LittleEndian);
  
  qint32 code=0; qint32 length=0;
  while(true){
    in_data >> code;
    in_data >> length;
    if(code == REND) {            in_data.skipRawData(length+header_remaining);}
    else {break;}
  }
  if(code != TEST) {
    kDebug(0) << "no thumbnail found" << endl;
    in_file.close();
    return false;
  }
  in_data.skipRawData(header_remaining);
  int x=0,y=0;
  in_data >> x;
  in_data >> y;

  if ((length-8) != x * y * 4) {
    in_file.close();
    return false;
  }
  char img_data[length-8];
  in_data.readRawData(img_data,length-8);
   QImage out_img((unsigned char*) img_data,x,y,QImage::Format_RGB32);
   img = out_img.mirrored();
//   if(!img.loadFromData(image_data)) {
// 	kDebug(0) << "img.load(): " << path << endl;
// 	return false;
//   }

//   if(img.depth() != 32) { img = img.convertToFormat(QImage::Format_RGB32);}

  kDebug(0) << "Thumbnail for " << path << " created" << endl;
  in_file.close();
  return true;
}

ThumbCreator::Flags BlendCreator::flags() const{
    return None;
}

