WITH kevin_bacon_id AS (
    SELECT id
    FROM people
    WHERE name = 'Kevin Bacon'
    AND birth = '1958'
)

SELECT name
FROM people
WHERE id IN (
    SELECT DISTINCT(person_id)
    FROM stars
    WHERE movie_id IN (
        SELECT id
        FROM movies
        WHERE id IN (
            SELECT movie_id
            FROM stars
            WHERE person_id = (SELECT id FROM kevin_bacon_id)
        )
    )
)
AND id NOT IN (SELECT id FROM kevin_bacon_id)
ORDER BY name;
