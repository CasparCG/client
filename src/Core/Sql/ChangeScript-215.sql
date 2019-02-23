UPDATE Configuration SET Name = 'EnableOscInputMonitor' WHERE Name = 'EnableOscInputMonitoring';
UPDATE Configuration SET Name = 'OscMonitorPort' WHERE Name = 'OscPort';
UPDATE Configuration SET Value = '3250' WHERE Name = 'OscMonitorPort';
INSERT INTO Configuration (Name, Value) VALUES('EnableOscInputWebSocket', 'true');
INSERT INTO Configuration (Name, Value) VALUES('OscControlPort', '6250');
