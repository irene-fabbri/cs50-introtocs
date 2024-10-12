-- Keep a log of any SQL queries you execute as you solve the mystery.
-- See what data I have
.tables

-- Examining the crime scene report
.schema crime_scene_reports

SELECT description
FROM crime_scene_reports
WHERE year = 2023
AND month = 7
AND day = 28
AND street = 'Humphrey Street';
-- Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time
-- – each of their interview transcripts mentions the bakery. Littering took place at 16:36. No known witnesses.

-- Examining the interview transcripts
.schema interviews

SELECT name, transcript
FROM interviews
WHERE year = 2023
AND month = 7
AND day = 28
AND transcript LIKE '%bakery%';
--| Ruth    | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away.
           -- If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.


--| Eugene  | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking
           --by the ATM on Leggett Street and saw the thief there withdrawing some money.

--| Raymond | As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say
           -- that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the
           -- phone to purchase the flight ticket.

-- Examining the secutity footage and find the license plate of the thief OR the accomplice
.schema bakery_security_logs

SELECT hour, minute, activity, license_plate
FROM bakery_security_logs
WHERE year = 2023
AND month = 7
AND day = 28
AND hour = 10
AND minute > 15
AND minute <= 25;

-- Examining the ATM transactions
.schema atm_transactions

SELECT account_number, amount
FROM atm_transactions
WHERE year = 2023
AND month = 7
AND day = 28
AND atm_location = 'Leggett Street'
AND transaction_type = 'withdraw';

-- Examining the secutity footage and find the license plate of the thief OR the accomplice
.schema bakery_security_logs

SELECT hour, minute, activity, license_plate
FROM bakery_security_logs
WHERE year = 2023
AND month = 7
AND day = 28
AND hour = 10
AND minute > 15
AND minute <= 25;

-- Examining the ATM transactions
.schema atm_transactions

SELECT account_number, amount
FROM atm_transactions
WHERE year = 2023
AND month = 7
AND day = 28
AND atm_location = 'Leggett Street'
AND transaction_type = 'withdraw';

-- Find if there are people who have one of these licence plates AND bank accounts
.schema people
.schema bank_accounts

--Find the flight
.schema flights
.schema airports
.schema passengers

-- At first, find the first flight leaving tomorrow from Fiftyville
SELECT id
FROM flights
WHERE origin_airport_id IN (
    SELECT id
    FROM airports
    WHERE city = 'Fiftyville'
)
AND year = 2023
AND month = 7
AND day = 29
ORDER BY hour , minute
LIMIT 1;

--Find the plane destination
SELECT city
FROM AIRPORTS
WHERE id = (
    SELECT destination_airport_id
    FROM flights
    WHERE origin_airport_id IN (
        SELECT id
        FROM airports
        WHERE city = 'Fiftyville'
    )
    AND year = 2023
    AND month = 7
    AND day = 29
    ORDER BY hour , minute
    LIMIT 1
);
-- Destination: NY

-- The Thief is the one who withdrew, called and is a passenger of the flight
SELECT name
FROM people
WHERE passport_number IN (
    SELECT passport_number
    FROM passengers
    WHERE flight_id = (
        SELECT id
        FROM flights
        WHERE origin_airport_id IN (
            SELECT id
            FROM airports
            WHERE city = 'Fiftyville'
        )
        AND year = 2023
        AND month = 7
        AND day = 29
        ORDER BY hour , minute
        LIMIT 1
    )
)
AND id IN (
    SELECT person_id
    FROM bank_accounts
    WHERE account_number IN (
        SELECT account_number
        FROM atm_transactions
        WHERE year = 2023
        AND month = 7
        AND day = 28
        AND atm_location = 'Leggett Street'
        AND transaction_type = 'withdraw'
    )
)
AND phone_number IN (
    SELECT caller
    FROM phone_calls
    WHERE year = 2023
    AND month = 7
    AND day = 28
    AND duration < 60
)
AND license_plate IN (
    SELECT license_plate
    FROM bakery_security_logs
    WHERE year = 2023
    AND month = 7
    AND day = 28
    AND hour = 10
    AND minute > 15
    AND minute <= 25
);

SELECT name
FROM people
WHERE phone_number IN (
    SELECT receiver
    FROM phone_calls
    WHERE year = 2023
    AND month = 7
    AND day = 28
    AND duration < 60
    AND caller IN (
        SELECT phone_number
        FROM people
        WHERE passport_number IN (
            SELECT passport_number
            FROM passengers
            WHERE flight_id = (
                SELECT id
                FROM flights
                WHERE origin_airport_id IN (
                    SELECT id
                    FROM airports
                    WHERE city = 'Fiftyville'
                )
                AND year = 2023
                AND month = 7
                AND day = 29
                ORDER BY hour , minute
                LIMIT 1
            )
        )
        AND id IN (
            SELECT person_id
            FROM bank_accounts
            WHERE account_number IN (
                SELECT account_number
                FROM atm_transactions
                WHERE year = 2023
                AND month = 7
                AND day = 28
                AND atm_location = 'Leggett Street'
                AND transaction_type = 'withdraw'
            )
        )
        AND license_plate IN (
            SELECT license_plate
            FROM bakery_security_logs
            WHERE year = 2023
            AND month = 7
            AND day = 28
            AND hour = 10
            AND minute > 15
            AND minute <= 25
        )
    )
);
