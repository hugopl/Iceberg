/*
    This file is part of Iceberg.

    Copyright (c) 2012 Anselmo L. S. Melo <anselmolsm@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef UTILS_H
#define UTILS_H

#include <QVector>
#include <QColor>

namespace Utils {

QVector<quint32> crc24Table;

void initCrc24Table()
{
    crc24Table << 0x00000000 << 0x00D6A776 << 0x00F64557 << 0x0020E221 << 0x00B78115 << 0x00612663 <<
                  0x0041C442 << 0x00976334 << 0x00340991 << 0x00E2AEE7 << 0x00C24CC6 << 0x0014EBB0 <<
                  0x00838884 << 0x00552FF2 << 0x0075CDD3 << 0x00A36AA5 << 0x00681322 << 0x00BEB454 <<
                  0x009E5675 << 0x0048F103 << 0x00DF9237 << 0x00093541 << 0x0029D760 << 0x00FF7016 <<
                  0x005C1AB3 << 0x008ABDC5 << 0x00AA5FE4 << 0x007CF892 << 0x00EB9BA6 << 0x003D3CD0 <<
                  0x001DDEF1 << 0x00CB7987 << 0x00D02644 << 0x00068132 << 0x00266313 << 0x00F0C465 <<
                  0x0067A751 << 0x00B10027 << 0x0091E206 << 0x00474570 << 0x00E42FD5 << 0x003288A3 <<
                  0x00126A82 << 0x00C4CDF4 << 0x0053AEC0 << 0x008509B6 << 0x00A5EB97 << 0x00734CE1 <<
                  0x00B83566 << 0x006E9210 << 0x004E7031 << 0x0098D747 << 0x000FB473 << 0x00D91305 <<
                  0x00F9F124 << 0x002F5652 << 0x008C3CF7 << 0x005A9B81 << 0x007A79A0 << 0x00ACDED6 <<
                  0x003BBDE2 << 0x00ED1A94 << 0x00CDF8B5 << 0x001B5FC3 << 0x00FB4733 << 0x002DE045 <<
                  0x000D0264 << 0x00DBA512 << 0x004CC626 << 0x009A6150 << 0x00BA8371 << 0x006C2407 <<
                  0x00CF4EA2 << 0x0019E9D4 << 0x00390BF5 << 0x00EFAC83 << 0x0078CFB7 << 0x00AE68C1 <<
                  0x008E8AE0 << 0x00582D96 << 0x00935411 << 0x0045F367 << 0x00651146 << 0x00B3B630 <<
                  0x0024D504 << 0x00F27272 << 0x00D29053 << 0x00043725 << 0x00A75D80 << 0x0071FAF6 <<
                  0x005118D7 << 0x0087BFA1 << 0x0010DC95 << 0x00C67BE3 << 0x00E699C2 << 0x00303EB4 <<
                  0x002B6177 << 0x00FDC601 << 0x00DD2420 << 0x000B8356 << 0x009CE062 << 0x004A4714 <<
                  0x006AA535 << 0x00BC0243 << 0x001F68E6 << 0x00C9CF90 << 0x00E92DB1 << 0x003F8AC7 <<
                  0x00A8E9F3 << 0x007E4E85 << 0x005EACA4 << 0x00880BD2 << 0x00437255 << 0x0095D523 <<
                  0x00B53702 << 0x00639074 << 0x00F4F340 << 0x00225436 << 0x0002B617 << 0x00D41161 <<
                  0x00777BC4 << 0x00A1DCB2 << 0x00813E93 << 0x005799E5 << 0x00C0FAD1 << 0x00165DA7 <<
                  0x0036BF86 << 0x00E018F0 << 0x00AD85DD << 0x007B22AB << 0x005BC08A << 0x008D67FC <<
                  0x001A04C8 << 0x00CCA3BE << 0x00EC419F << 0x003AE6E9 << 0x00998C4C << 0x004F2B3A <<
                  0x006FC91B << 0x00B96E6D << 0x002E0D59 << 0x00F8AA2F << 0x00D8480E << 0x000EEF78 <<
                  0x00C596FF << 0x00133189 << 0x0033D3A8 << 0x00E574DE << 0x007217EA << 0x00A4B09C <<
                  0x008452BD << 0x0052F5CB << 0x00F19F6E << 0x00273818 << 0x0007DA39 << 0x00D17D4F <<
                  0x00461E7B << 0x0090B90D << 0x00B05B2C << 0x0066FC5A << 0x007DA399 << 0x00AB04EF <<
                  0x008BE6CE << 0x005D41B8 << 0x00CA228C << 0x001C85FA << 0x003C67DB << 0x00EAC0AD <<
                  0x0049AA08 << 0x009F0D7E << 0x00BFEF5F << 0x00694829 << 0x00FE2B1D << 0x00288C6B <<
                  0x00086E4A << 0x00DEC93C << 0x0015B0BB << 0x00C317CD << 0x00E3F5EC << 0x0035529A <<
                  0x00A231AE << 0x007496D8 << 0x005474F9 << 0x0082D38F << 0x0021B92A << 0x00F71E5C <<
                  0x00D7FC7D << 0x00015B0B << 0x0096383F << 0x00409F49 << 0x00607D68 << 0x00B6DA1E <<
                  0x0056C2EE << 0x00806598 << 0x00A087B9 << 0x007620CF << 0x00E143FB << 0x0037E48D <<
                  0x001706AC << 0x00C1A1DA << 0x0062CB7F << 0x00B46C09 << 0x00948E28 << 0x0042295E <<
                  0x00D54A6A << 0x0003ED1C << 0x00230F3D << 0x00F5A84B << 0x003ED1CC << 0x00E876BA <<
                  0x00C8949B << 0x001E33ED << 0x008950D9 << 0x005FF7AF << 0x007F158E << 0x00A9B2F8 <<
                  0x000AD85D << 0x00DC7F2B << 0x00FC9D0A << 0x002A3A7C << 0x00BD5948 << 0x006BFE3E <<
                  0x004B1C1F << 0x009DBB69 << 0x0086E4AA << 0x005043DC << 0x0070A1FD << 0x00A6068B <<
                  0x003165BF << 0x00E7C2C9 << 0x00C720E8 << 0x0011879E << 0x00B2ED3B << 0x00644A4D <<
                  0x0044A86C << 0x00920F1A << 0x00056C2E << 0x00D3CB58 << 0x00F32979 << 0x00258E0F <<
                  0x00EEF788 << 0x003850FE << 0x0018B2DF << 0x00CE15A9 << 0x0059769D << 0x008FD1EB <<
                  0x00AF33CA << 0x007994BC << 0x00DAFE19 << 0x000C596F << 0x002CBB4E << 0x00FA1C38 <<
                  0x006D7F0C << 0x00BBD87A << 0x009B3A5B << 0x004D9D2D;
}

quint32 crc24(const QString &data)
{
    quint32 polinomial = 0x00ffffff;
    quint32 current = 0;
    QString::const_iterator iter;
    for (iter = data.constBegin(); iter != data.constEnd(); ++iter) {
        current = ((polinomial & 0x0000ffff) ^ (*iter).unicode()) & 0xff;
        polinomial = (polinomial >> 8) ^ crc24Table[current];
    }
    return ~polinomial & 0x00ffffff;
}

QColor colorByName(const QString &name)
{
    const quint32 polinomial = crc24(name);

    const quint32 r =  255- ((polinomial & 0x0000ffff) & 0xff);
    const quint32 g = 255 - (((polinomial >> 8) & 0x0000ffff) & 0xff);
    const quint32 b = 255 -(((polinomial >> 16) & 0x0000ffff) & 0xff);

    return QColor(r, g, b);
}

}

#endif // UTILS_H
