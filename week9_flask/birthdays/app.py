import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":

        # TODO: Add the user's entry into the database
        NAME = request.form.get("name")
        MONTH = request.form.get("month")
        DAY = request.form.get("day")

        if not NAME or not MONTH or not DAY:
            redirect("/")

        try:
            MONTH = int(MONTH)
        except ValueError:
            return redirect("/")
        if MONTH < 1 or MONTH > 12:
            return redirect("/")

        try:
            DAY = int(DAY)
        except ValueError:
            return redirect("/")
        if DAY < 1 or DAY > 31:
            return redirect("/")

        db.execute("INSERT INTO birthdays (name, month, day) VALUES(?,?,?)", NAME, MONTH, DAY)

        return redirect("/")

    else:

        # TODO: Display the entries in the database on index.html

        BIRTHDAYS = db.execute("SELECT * FROM birthdays")

        return render_template("index.html", birthdays=BIRTHDAYS)

