CREATE TABLE OpenRecent (Id INTEGER PRIMARY KEY, Value TEXT UNIQUE);

CREATE TABLE AtemMixerStep (Id INTEGER PRIMARY KEY, Name TEXT, Value TEXT);
INSERT INTO AtemMixerStep (Name, Value) VALUES('1', '0');
INSERT INTO AtemMixerStep (Name, Value) VALUES('2', '1');

ALTER TABLE Device ADD COLUMN LockedChannel INTEGER DEFAULT 0;

DELETE FROM Chroma WHERE Key = 'Red'
DELETE FROM Chroma WHERE Key = 'Yellow'
DELETE FROM Chroma WHERE Key = 'Torquise'
DELETE FROM Chroma WHERE Key = 'Magenta'
