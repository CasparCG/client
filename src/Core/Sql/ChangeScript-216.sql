INSERT INTO OscOutput (Name, Address, Port, Description) VALUES('Localhost', '127.0.0.1', 3250, '');

UPDATE Configuration SET Value = '3250' WHERE Name = 'OscControlPort';
UPDATE Configuration SET Value = '6250' WHERE Name = 'OscMonitorPort';
