#include "blendercreator.h"
#include <QFile>
#include <qdatastream.h>
#include <QImage>
#include <assert.h>
#include <zlib.h>
#include <QLoggingCategory>
Q_DECLARE_LOGGING_CATEGORY(LOG_KIO_BLEND)
Q_LOGGING_CATEGORY(LOG_KIO_BLEND, "kio_blender")
#include <qbuffer.h>
// #include <KFilterDev>
#include <KCompressionDevice>


extern "C" {
    
  Q_DECL_EXPORT ThumbCreator *new_creator() {
    return new BlendCreator;
  }

}


BlendCreator::BlendCreator() {}



bool BlendCreator::create(const QString& path, int width , int height, QImage& img){

  const qint32 REND = 1145980242; // "REND" as signed 32 bit number
  const qint32 TEST = 1414743380; //"TEST" as signed 32 bit number
  QFile in_file (path);

  if(!in_file.open(QIODevice::ReadOnly)) {
    qCDebug(LOG_KIO_BLEND) << "blend.open" << path << endl;
    return false;
  }
  KCompressionDevice * gz_file;
  QDataStream in_data;
  char zipmagick[2];
  qint64 z = in_file.peek(zipmagick,2);
  
    unsigned char zm2 = zipmagick[1];
    unsigned char zm1 = zipmagick[0];
  if(z && zm1==0x1f && zm2 == 0x8b) {
    in_file.close();
    gz_file = new KCompressionDevice(path, KCompressionDevice::GZip);
    gz_file->open(QIODevice::ReadOnly);
    in_data.setDevice(gz_file);
  } else {
    in_data.setDevice(&in_file);
  }
  char magic[7];
  in_data.readRawData(magic,7);
//   printf("magic %s\n",magic);
  QByteArray magick(magic);
  if( !magick.startsWith("BLENDER")){
//     !strcmp(magic,"BLENDER",7)){
// 	printf ("not a blend file\n");
    qCDebug(LOG_KIO_BLEND) << "not a blend file " << path << endl;
    in_file.close();
    return false;
  }
  qint8 is_64_bit = 0;
  in_data >> is_64_bit;
//   printf("64 bits %i\n",is_64_bit);
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
    if(code == REND) {
// 	  printf("render section skipping %i + %i\n",length, header_remaining);
	  char buf[length+header_remaining];
	  in_data.readRawData(buf,length+header_remaining);
	}
    else {break;}
  }
  if(code != TEST) {
    qCDebug(LOG_KIO_BLEND) << "no thumbnail found" << endl;
    in_file.close();
    return false;
  }
//   printf("found relevent section code %i skipping %i\n", code, header_remaining);
  char buf[header_remaining];
  in_data.readRawData(buf,header_remaining);
  quint32 x=0,y=0;
  in_data >> x;
  in_data >> y;
//   printf("x %i,y %i\n",x,y);

  if ((length-8) != x * y * 4) {
    in_file.close();
    return false;
  }
  char img_data[length-8];
  in_data.readRawData(img_data,length-8);
  abgr_to_argb(img_data,length-8);
   QImage out_img((unsigned char*) img_data,x,y,QImage::Format_ARGB32);
   if(width != 128) { out_img = out_img.scaledToWidth(width,Qt::SmoothTransformation); }
   if(height != 128) { out_img = out_img.scaledToHeight(height,Qt::SmoothTransformation); }
   out_img = out_img.mirrored();
   img = out_img.convertToFormat(QImage::Format_ARGB32_Premultiplied);

  qCDebug(LOG_KIO_BLEND) << "Thumbnail for " << path << " created" << endl;
  in_file.close();
  return true;
}

void BlendCreator::abgr_to_argb(char* buff, int size) const {
  char tmp;
  for (int i=0; i<size;i+=4) {
	tmp = buff[i];
	buff[i] = buff[i+2];
	buff[i+2] = tmp;
  }
}



ThumbCreator::Flags BlendCreator::flags() const{
    return None;
}

