//#include <QCoreApplication>
#include <QImage>
#include <QPainter>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QRandomGenerator>

int main(int argc, char *argv[])
{
    //test-emulator.exe [INPUT IMAGE] [PERCENT OF "BUGS"] [OUTPUT FOLDER]

    QString dstFolder(argv[3]);
    QString dstImgFileName(dstFolder + "/img.png");
    QString dstTxtFileName(dstFolder + "/stat.txt");

    QFile::remove(dstImgFileName);
    QFile::remove(dstTxtFileName);

    if(argc < 3) return -1;

    //Load image
    QImage *img = new QImage(argv[1]);
    QPainter p(img);

    //Draw red rectangle in 10% cases
    int bugPercent = QString(argv[2]).toInt();
    int randValue = QRandomGenerator::global()->generate() % 100;
    if(randValue < bugPercent) {
      //Generate bug
      randValue = QRandomGenerator::global()->generate() % 2;
      if(randValue == 0) {
          return -1;
      } else {
          int w = img->width()/2;
          int h = img->height()/2;
          p.setBrush(QBrush(Qt::red));
          p.setPen(Qt::red);
          p.drawRect(0,0,w,h);
          p.drawRect(w,h,w,h);
      }
    }
    p.end();

    //Count pixels with red
    long redPixCount = 0;
    for(int i = 0; i < img->width(); i++)
        for(int j = 0; j < img->height(); j++) {
            QRgb c = img->pixel(i,j);
            if(qRed(c) > 128) ++redPixCount;
        }


    //Save image
    img->save(dstImgFileName);

    //Output text to console and to text file
    QFile txtFile(dstTxtFileName);
    QTextStream out(&txtFile);
    if(txtFile.open(QIODevice::WriteOnly)) {
        out << "Red pixels: " + QString::number(redPixCount) + "\n";
        printf("Red pixels: %lu\n", redPixCount);
        out << "Height (px): " + QString::number(img->height()) + "\n";
        printf("Height (px): %i\n", img->height());
        out << "Height (mm): " + QString::number(img->heightMM()) + "\n";
        printf("Height (mm): %i\n", img->heightMM());
        out << "Width (px): " + QString::number(img->width()) + "\n";
        printf("Width (px): %i\n", img->width());
        out << "Width (mm): " + QString::number(img->widthMM()) + "\n";
        printf("Width (mm): %i\n", img->widthMM());
    }
    txtFile.close();

    //QCoreApplication a(argc, argv);
    //return a.exec();

    delete img;
    printf("The END.");
}
