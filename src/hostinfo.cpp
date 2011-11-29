/*
    This file is part of Icecream.

    Copyright (c) 2004 Cornelius Schumacher <schumacher@kde.org>
    Copyright (c) 2011 Hugo Parente Lima <hugo.pl@gmail.com>

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

#include "hostinfo.h"
#include <assert.h>
#include <QDebug>

QVector<QColor> HostInfo::mColorTable;
QMap<int,QString> HostInfo::mColorNameMap;

void HostInfo::initColorTable()
{
    initColor( "#A5080B", tr("cherry") );
    initColor( "#76d26f", tr("pistachio"));
    initColor( "#664a08", tr("chocolate"));
    initColor( "#4c9dff", tr("smurf"));
    initColor( "#6c2ca8", tr("blueberry"));
    initColor( "#fa8344", tr("orange"));
    initColor( "#55CFBD", tr("mint"));
    initColor( "#db1230", tr("strawberry"));
    initColor( "#a6ea5e", tr("apple"));
    initColor( "#D6A3D8", tr("bubblegum"));
    initColor( "#f2aa4d", tr("peach"));
    initColor( "#aa1387", tr("plum"));
    initColor( "#26c3f7", tr("polar sea"));
    initColor( "#b8850e", tr("nut"));
    initColor( "#6a188d", tr("blackberry"));
    initColor( "#24b063", tr("woodruff"));
    initColor( "#ffff0f", tr("banana"));
    initColor( "#1e1407", tr("mocha"));
    initColor( "#29B450", tr("kiwi"));
    initColor( "#F8DD31", tr("lemon"));
    initColor( "#fa7e91", tr("raspberry"));
    initColor( "#c5a243", tr("caramel"));
    initColor( "#b8bcff", tr("blueberry"));
    // try to make the count a prime number (reminder: 19, 23, 29, 31)
    // initColor( "#ffb8c0", tr("blackcurrant"));
    // initColor( "#f7d36f", tr("passionfruit"));
    // initColor( "#d51013", tr("pomegranate"));
    // initColor( "#C2C032", tr("pumpkin" ) );
}

void HostInfo::initColor( const QString &value , const QString &name )
{
    QColor c( value );
    mColorTable.append( c );

    mColorNameMap.insert( c.red() + c.green() * 256 + c.blue() * 65536, name );
}

QString HostInfo::colorName( const QColor &c )
{
  int key = c.red() + c.green() * 256 + c.blue() * 65536;

  return mColorNameMap.value( key, tr("<unknown>") );
}

HostInfo::HostInfo( unsigned int id )
  : mId( id )
{
}

unsigned int HostInfo::id() const
{
  return mId;
}

QColor HostInfo::color() const
{
  return mColor;
}

QString HostInfo::name() const
{
  return mName;
}

QString HostInfo::ip() const
{
  return mIp;
}

QString HostInfo::platform() const
{
    return mPlatform;
}

unsigned int HostInfo::maxJobs() const
{
  return mMaxJobs;
}

bool HostInfo::isOffline() const
{
  return mOffline;
}

float HostInfo::serverSpeed() const
{
    return mServerSpeed;
}

unsigned int HostInfo::serverLoad() const
{
    return mServerLoad;
}

void HostInfo::updateFromStatsMap( const StatsMap &stats )
{
  const QString& name = stats["Name"];

  if ( name != mName ) {
    mName = name;
    mColor = createColor( mName );
    mIp = stats["IP"];
    mPlatform = stats["Platform"];
  }

  mMaxJobs = stats["MaxJobs"].toUInt();
  mOffline = ( stats["State"] == "Offline" );

  mServerSpeed = stats["Speed"].toFloat();

  mServerLoad = stats["Load"].toUInt();
}

QColor HostInfo::createColor( const QString &name )
{
    unsigned long h = 0;
    unsigned long g;
    int ch;

    for( uint i = 0; i < (uint)name.length(); ++i ) {
        ch = name[i].unicode();
        h = (h << 4) + ch;
        if ((g = (h & 0xf0000000)) != 0)
        {
            h ^= g >> 24;
            h ^= g;
        }
    }

    h += name.length() + ( name.length() << 17 );
    h ^= h >> 2;

    return mColorTable[ h % mColorTable.count() ];
}

QColor HostInfo::createColor()
{
  static int num = 0;

  return mColorTable.at( num++ % mColorTable.count() );
}

HostInfoManager::HostInfoManager()
{
  HostInfo::initColorTable();
}

HostInfoManager::~HostInfoManager()
{
  qDeleteAll(mHostMap);
}

HostInfo *HostInfoManager::find( unsigned int hostid ) const
{
  return mHostMap.value( hostid, 0 );
}

HostInfo *HostInfoManager::checkNode( unsigned int hostid,
                                      const HostInfo::StatsMap &stats )
{
  HostMap::ConstIterator it = mHostMap.constFind( hostid );
  HostInfo *hostInfo;
  if ( it == mHostMap.constEnd() ) {
    hostInfo = new HostInfo( hostid );
    mHostMap.insert( hostid, hostInfo );
  } else {
    hostInfo = *it;
  }

  hostInfo->updateFromStatsMap( stats );

  return hostInfo;
}

QString HostInfoManager::nameForHost( unsigned int id ) const
{
  HostInfo *hostInfo = find( id );
  if ( hostInfo ) return hostInfo->name();

  return tr("<unknown>");
}

QColor HostInfoManager::hostColor( unsigned int id ) const
{
  if ( id ) {
    HostInfo *hostInfo = find( id );
    if ( hostInfo ) {
        QColor tmp = hostInfo->color();
        assert( tmp.isValid() && ( tmp.red() + tmp.green() + tmp.blue() ));
        return tmp;
    }
  }

  qWarning() << "id " << id << " got no color\n";
  assert( false );

  return QColor( 0, 0, 0 );
}

unsigned int HostInfoManager::maxJobs( unsigned int id ) const
{
  if ( id ) {
    HostInfo *hostInfo = find( id );
    if ( hostInfo ) return hostInfo->maxJobs();
  }

  return 0;
}

HostInfoManager::HostMap HostInfoManager::hostMap() const
{
  return mHostMap;
}

void HostInfoManager::setSchedulerName( const QString& schedulerName )
{
    mSchedulerName = schedulerName;
}

void HostInfoManager::setNetworkName( const QString& networkName )
{
    mNetworkName = networkName;
}

