PRAGMA table_info(features);
--select maximumLocationMZ from features order by maximumLocationMZ asc limit 10;

-- checking the mz's
select MZ, maximumLocationMZ, centerOfMassMZ from features limit 10;

-- checking the rt's
select RT, maximumLocationRT, centerOfMassRT from features limit 10;

-- checking the intensities;
select Max, Volume, Count from features limit 10;

-- features statistics mz;
select MZSD,MZSKEW,MZKURT from features limit 10;

-- features statistics rt;
select RTSD,RTSKEW,RTKURT from features limit 10;

-- id's
select id, idswath from features limit 10;

-- pixel locations:
select minRTIndex, rtExtend, minMZIndex, mzExtend from features limit 10;

/* RTree queries */

-- check RT's
PRAGMA table_info(feature_index);
SELECT id From feature_index order by id asc limit 10 ;
SELECT minRT, maxRT FROM feature_index limit 10;
SELECT minMZ, maxMZ FROM feature_index limit 10;
SELECT minMZsw, maxMZsw FROM feature_index limit 10;


select min(minRT), max(maxRT) from feature_index limit 10;
select min(RT), max(RT) from features limit 10;
select min(minMZ), max(maxMZ) from feature_index limit 10;


/* RTree queries */
SELECT count(id) FROM feature_index WHERE maxRT >= 1020 AND minRT <= 1020 AND maxMZ >= 500 AND minMZ <= 700 limit 10;
SELECT count(id) FROM feature_index WHERE  maxRT >= 1019 AND minRT <= 1020;
SELECT count(id) FROM feature_index WHERE minRT <= 1019 AND maxRT >= 1020;


create view test2 as select features.id, features.RT, featid.maxRT from features, (select * from feature_index where minRT <= 1019 AND maxRT >= 1020) as featid where features.id = featid.id;
select count(*) from test2;


Create view test as select features.id, features.RT, featid.maxRT from features, feature_index as featid where features.id = featid.id and (featid.minRT <= 1019 and featid.maxRT >= 1020);
select count(*) from test;


