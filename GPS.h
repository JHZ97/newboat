#include <QString>
#ifndef GPS_H
#define GPS_H

class GPS
{
public:
    //double raw_GPS_to_double(double raw);
    GPS(double lon,double lat): lon(lon),lat(lat){}
    GPS(QString lon,QString lat): lon(lon.toDouble()),lat(lat.toDouble()){}
    void GPS_to_BD();
    void BD_to_GPS();
    double lon,lat;
private:
    void gps84_To_Gcj02(double& lat, double& lon);
    void gcj02_To_Bd09(double& gg_lat, double& gg_lon);
    void Bd09_To_gcj02(double&,double&);
    void gcj02_To_gps84(double&,double&);
    double transformLat(double,double);
    double transformLon(double,double);
};

#endif // GPS_H
