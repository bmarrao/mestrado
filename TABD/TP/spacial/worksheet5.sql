select count(id) from taxi_stands;
SELECT ST_AsText(location)FROM taxi_stands WHERE name = 'Infante';
SELECT name, ST_Y(location) FROM taxi_stands Order by ST_Y(location) desc limit 1;
SELECT name, ST_Y(location) FROM taxi_stands Order by ST_Y(location) limit 1;
SELECT name, ST_X(location) FROM taxi_stands Order by ST_X(location) desc limit 1;
SELECT name, ST_X(location) FROM taxi_stands Order by ST_X(location) limit 1;

SELECT ST_AsText(ST_MakeEnvelope(
    (SELECT MIN(ST_X(location)) FROM taxi_stands), -- Minimum X (most west)
    (SELECT MIN(ST_Y(location)) FROM taxi_stands), -- Minimum Y (most south)
    (SELECT MAX(ST_X(location)) FROM taxi_stands), -- Maximum X (most east)
    (SELECT MAX(ST_Y(location)) FROM taxi_stands)  -- Maximum Y (most north)
)) AS rectangle_geometry;

SELECT ST_Area(ST_MakeEnvelope(
    (SELECT MIN(ST_X(location)) FROM taxi_stands), -- Minimum X (most west)
    (SELECT MIN(ST_Y(location)) FROM taxi_stands), -- Minimum Y (most south)
    (SELECT MAX(ST_X(location)) FROM taxi_stands), -- Maximum X (most east)
    (SELECT MAX(ST_Y(location)) FROM taxi_stands)  -- Maximum Y (most north)
)) AS area;

SELECT 
    ts1.name AS taxi_stand1, 
    ts2.name AS taxi_stand2, 
    ST_Distance(ts1.location, ts2.location) AS distance
FROM 
    taxi_stands ts1
JOIN 
    taxi_stands ts2 
ON 
    ts1.name < ts2.name -- Avoid duplicate pairs
WHERE 
    ts1.name != ts2.name -- Exclude pairs of the same taxi stand
ORDER BY 
    distance
LIMIT 
    1;

SELECT ST_Centroid(ST_MakeEnvelope(
    (SELECT MIN(ST_X(location)) FROM taxi_stands), -- Minimum X (most west)
    (SELECT MIN(ST_Y(location)) FROM taxi_stands), -- Minimum Y (most south)
    (SELECT MAX(ST_X(location)) FROM taxi_stands), -- Maximum X (most east)
    (SELECT MAX(ST_Y(location)) FROM taxi_stands)  -- Maximum Y (most north)
)) AS centroid;

SELECT 
    ts1.name AS taxi_stand1, 
    ts2.name AS taxi_stand2, 
    ST_Distance(ts1.location, ts2.location) AS distance
FROM 
    taxi_stands ts1
JOIN 
    taxi_stands ts2 
ON 
    ts1.name < ts2.name -- Avoid duplicate pairs
WHERE 
    ts1.name != ts2.name -- Exclude pairs of the same taxi stand
ORDER BY 
    distance desc
LIMIT 
    1;

SELECT ST_Distance(location, ST_Centroid(ST_MakeEnvelope(
    (SELECT MIN(ST_X(location)) FROM taxi_stands), -- Minimum X (most west)
    (SELECT MIN(ST_Y(location)) FROM taxi_stands), -- Minimum Y (most south)
    (SELECT MAX(ST_X(location)) FROM taxi_stands), -- Maximum X (most east)
    (SELECT MAX(ST_Y(location)) FROM taxi_stands)  -- Maximum Y (most north)
))) from taxi_stands

SELECT name FROM taxi_stands WHERE ST_Distance(location, ST_Centroid(ST_MakeEnvelope(
    (SELECT MIN(ST_X(location)) FROM taxi_stands), -- Minimum X (most west)
    (SELECT MIN(ST_Y(location)) FROM taxi_stands), -- Minimum Y (most south)
    (SELECT MAX(ST_X(location)) FROM taxi_stands), -- Maximum X (most east)
    (SELECT MAX(ST_Y(location)) FROM taxi_stands)  -- Maximum Y (most north)
)) ) < 1



