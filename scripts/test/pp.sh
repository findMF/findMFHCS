#./PeakPicker --in /home/witold/testdata/luca/Boris_MS1_100_400_245_250.mzML --out /home/witold/testdata/luca/Boris_MS1_100_400_245_250.c.mzML --resolution=30000 --smoothwidth=3.5 --intwidth=2 --area=1 --ppstate=0
#./PeakPicker --in /home/witold/testdata/luca/Boris_MS1_100_400_245_250.mzML --out /home/witold/testdata/luca/Boris_MS1_100_400_245_250.c.mzML --resolution=30000 --smoothwidth=3.5 --intwidth=2 --area=0 --ppstate=0

#./PeakPicker --in /home/witold/testdata/luca/TripleTOF30003100.mzML --out /home/witold/testdata/luca/TripleTOF30003100.a.c.mzML --resolution=30000 --smoothwidth=2.5 --intwidth=2 --area=1 --ppstate=0
#./PeakPicker --in /home/witold/testdata/luca/TripleTOF30003100.mzML --out /home/witold/testdata/luca/TripleTOF30003100.p.c.mzML --resolution=30000 --smoothwidth=2.5 --intwidth=2 --area=0 --ppstate=0

./PeakPicker --in /home/witold/testdata/luca/TripleTOF30003100.mzML --out /home/witold/testdata/luca/TripleTOF30003100.r.s.mzML --resolution=30000 --smoothwidth=2.5 --intwidth=2 --area=1 --ppstate=1
./PeakPicker --in /home/witold/testdata/luca/TripleTOF30003100.mzML --out /home/witold/testdata/luca/TripleTOF30003100.s.s.mzML --resolution=30000 --smoothwidth=2.5 --intwidth=2 --area=1 --ppstate=1


./PeakPicker --in /home/witold/testdata/ludovic/lgillet_L121127_001_p_msconvert.mzXML --out  /home/witold/testdata/ludovic/lgillet_L121127_001_p_msconvert.i.c.mzML --resolution=30000 --smoothwidth=2.5 --intwidth=2 --area=0 --ppstate=0
./PeakPicker --in /home/witold/testdata/ludovic/lgillet_L121127_002_p_msconvert.mzXML --out  /home/witold/testdata/ludovic/lgillet_L121127_002_p_msconvert.i.c.mzML --resolution=30000 --smoothwidth=2.5 --intwidth=2 --area=0 --ppstate=0
./PeakPicker --in /home/witold/testdata/ludovic/lgillet_L121127_003_p_msconvert.mzXML --out  /home/witold/testdata/ludovic/lgillet_L121127_003_p_msconvert.i.c.mzML --resolution=30000 --smoothwidth=2.5 --intwidth=2 --area=0 --ppstate=0

./PeakPicker --in /home/witold/testdata/ludovic/lgillet_L121127_001_p_msconvert.mzXML --out  /home/witold/testdata/ludovic/lgillet_L121127_001_p_msconvert.a3.c.mzXML --resolution=30000 --smoothwidth=2.5 --intwidth=4 --area=1 --threshold=30 &
./PeakPicker --in /home/witold/testdata/ludovic/lgillet_L121127_002_p_msconvert.mzXML --out  /home/witold/testdata/ludovic/lgillet_L121127_002_p_msconvert.a3.c.mzXML --resolution=30000 --smoothwidth=2.5 --intwidth=4 --area=1 --threshold=30 &
./PeakPicker --in /home/witold/testdata/ludovic/lgillet_L121127_003_p_msconvert.mzXML --out  /home/witold/testdata/ludovic/lgillet_L121127_003_p_msconvert.a3.c.mzXML --resolution=30000 --smoothwidth=2.5 --intwidth=4 --area=1 --threshold=30 &

./PeakPicker --resolution=30000 --smoothwidth=2.5 --intwidth=4 --area=1 --threshold=30 --in /home/witold/testdata/ludovic/nselevse_L120327_001_msconvert_p.mzXML --out /home/witold/testdata/ludovic/nselevse_L120327_001_msconvert_p.a.mzXML &
./PeakPicker --resolution=30000 --smoothwidth=2.5 --intwidth=4 --area=1 --threshold=30 --in /home/witold/testdata/ludovic/nselevse_L120327_010_msconvert_p.mzXML --out /home/witold/testdata/ludovic/nselevse_L120327_010_msconvert_p.a.mzXML &
./PeakPicker --resolution=30000 --smoothwidth=2.5 --intwidth=4 --area=1 --threshold=30 --in /home/witold/testdata/ludovic/nselevse_L120327_016_msconvert_p.mzXML --out /home/witold/testdata/ludovic/nselevse_L120327_016_msconvert_p.a.mzXML &


./PeakPicker --resolution=30000 --smoothwidth=2.5 --intwidth=4 --area=0 --threshold=3 --in /home/witold/testdata/ludovic/nselevse_L120327_001_msconvert_p.mzXML --out /home/witold/testdata/ludovic/nselevse_L120327_001_msconvert_p.i.mzXML &
./PeakPicker --resolution=30000 --smoothwidth=2.5 --intwidth=4 --area=0 --threshold=3 --in /home/witold/testdata/ludovic/nselevse_L120327_010_msconvert_p.mzXML --out /home/witold/testdata/ludovic/nselevse_L120327_010_msconvert_p.i.mzXML &
./PeakPicker --resolution=30000 --smoothwidth=2.5 --intwidth=4 --area=0 --threshold=3 --in /home/witold/testdata/ludovic/nselevse_L120327_016_msconvert_p.mzXML --out /home/witold/testdata/ludovic/nselevse_L120327_016_msconvert_p.i.mzXML &

./PeakPicker --in /home/witold/testdata/ludovic/lgillet_L121127_003_p_msconvert.mzXML --out  /home/witold/testdata/ludovic/lgillet_L121127_003_p_msconvert.i.c.mzML --resolution=30000 --smoothwidth=2.5 --intwidth=3 --area=0 


#./PeakPicker --resolution=35000 --smoothwidth=2.5 --intwidth=5 --area=1 --threshold=6 --in /home/witold/testdata/ludovic/nselevse_L120327_001_msconvert_p.mzXML --out /home/witold/testdata/ludovic/nselevse_L120327_001_msconvert_p.35000i5.mzXML &
./PeakPicker --resolution=30000 --smoothwidth=2.5 --intwidth=4 --area=1 --threshold=6 --in /home/witold/testdata/ludovic/nselevse_L120327_001_msconvert_p.mzXML --out /home/witold/testdata/ludovic/nselevse_L120327_001_msconvert_p.30000i4.mzXML &
./PeakPicker --resolution=30000 --smoothwidth=2.5 --intwidth=4 --area=1 --threshold=6 --in /home/witold/testdata/ludovic/nselevse_L120327_010_msconvert_p.mzXML --out /home/witold/testdata/ludovic/nselevse_L120327_010_msconvert_p.30000i4.mzXML &
./PeakPicker --resolution=30000 --smoothwidth=2.5 --intwidth=4 --area=1 --threshold=6 --in /home/witold/testdata/ludovic/nselevse_L120327_016_msconvert_p.mzXML --out /home/witold/testdata/ludovic/nselevse_L120327_016_msconvert_p.30000i4.mzXML &

#./PeakPicker --resolution=25000 --smoothwidth=1.5 --intwidth=3 --area=1 --threshold=6 --in /home/witold/testdata/ludovic/nselevse_L120327_001_msconvert_p.mzXML --out /home/witold/testdata/ludovic/nselevse_L120327_001_msconvert_p.25000i3.mzXML &




