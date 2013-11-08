#ifndef WRITEMAPINFO_H
#define WRITEMAPINFO_H


///
struct WriteMapInfo{
  soci::session sql_;
  WriteMapInfo(const std::string & fileloc):sql_(soci::sqlite3,fileloc)
  {}

  ~WriteMapInfo(){
    sql_.close();
  }
  void writeMapInfo(){}
  /**
  -- one sample can have many maps 1:N
  drop table if exists mapinfo;
  CREATE TABLE mapinfo(
  id integer primary key,  -- id of map
  sampleid int not null unique, -- sample id
  scancount int not null, --  -- number of spectra in map
  mslevel int not null,  --  -- ms level of map
  mzminext real,  -- min extraction mass
  mzmaxext real, -- max extraction mass
  mzmin real not null, -- mz min of map
  mzmax real not null, --
  rtmin real not null, -- rt min of map
  rtmax real not null, --
  FOREIGN KEY(sampleid) REFERENCES sample(id)
  );
*/

  /**
  -- mappings mass and rt
  drop table if exists mapinfoaxis;
  CREATE TABLE mapinfoaxis (
  id integer primary key,
  idmapinfo int not null,  --  links to map info
  mzaxis blob, -- mass axis
  rtaxis blob,
  FOREIGN KEY(id) REFERENCES mapinfo(id)
  );
  */

};

#endif // WRITEMAPINFO_H
