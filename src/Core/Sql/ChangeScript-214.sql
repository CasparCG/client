UPDATE Configuration SET Name = 'EnableOscInputMonitoring' WHERE Name = 'EnableOscInput';
INSERT INTO Configuration (Name, Value) VALUES('EnableOscInputControl', 'true');
INSERT INTO Configuration (Name, Value) VALUES('OscWebSocketPort', '4250');
