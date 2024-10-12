import os
import datetime
import pytz
from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    STOCKS = db.execute(
        "SELECT symbol, SUM(shares) AS shares FROM transactions GROUP BY symbol HAVING user_id = ? AND SUM(shares)>0", session["user_id"])
    TOTAL = 0
    for stock in STOCKS:
        quoted = lookup(stock["symbol"])
        stock["price"] = quoted["price"]
        stock["total"] = quoted["price"] * stock["shares"]
        TOTAL += stock["total"]
    CASH = db.execute("SELECT cash FROM users WHERE id=?", session["user_id"])
    CASH = CASH[0]["cash"]
    TOTAL += CASH
    return render_template("index.html", cash=CASH, total=TOTAL, stocks=STOCKS)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    USER = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
    CASH = int(USER[0]["cash"])
    if request.method == "POST":
        SYMBOL = request.form.get("symbol")
        try:
            SHARES = int(request.form.get("shares"))
            if SHARES < 1:
                return apology("invalid share", 400)
        except ValueError:
            return apology("invalid share", 400)
        QUOTED = lookup(SYMBOL)
        if not QUOTED:
            return apology("invalid symbol")
        elif int(QUOTED["price"]) * SHARES > CASH:
            return apology("sorry, not rich enough for that", 400)
        else:
            # update transactions database
            PRICE = int(QUOTED["price"]) * SHARES
            db.execute("INSERT INTO transactions (date_time, symbol, shares, amount, user_id) VALUES(?,?,?,?,?)", datetime.datetime.now(
                pytz.timezone("US/Eastern")).strftime("%Y/%m/%d, %H:%M:%S"), SYMBOL.upper(), SHARES, PRICE, session["user_id"])
            # update user database with actual available cash
            db.execute("UPDATE users SET cash = cash-? WHERE id = ?", PRICE, session["user_id"])
            return redirect("/")
    else:
        return render_template("buy.html", user=USER[0])


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    STOCKS = db.execute(
        "SELECT symbol, shares, amount, date_time FROM transactions WHERE user_id = ? ORDER BY date_time ASC", session["user_id"])
    return render_template("history.html", stocks=STOCKS)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 400)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        SYMBOL = request.form.get("symbol")
        QUOTED = lookup(SYMBOL)
        if not QUOTED:
            return apology("invalid symbol")
        else:
            return render_template("quoted.html", quoted=QUOTED)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        USERNAME = request.form.get("username")
        PASSWORD = generate_password_hash(request.form.get("password"))
        CONFIRMATION = request.form.get("confirmation")

        # Ensure username was submitted
        if not USERNAME:
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not PASSWORD:
            return apology("must provide password", 400)
        elif not CONFIRMATION:
            return apology("must provide confirmation password", 400)
        elif not check_password_hash(PASSWORD, CONFIRMATION):
            return apology("passwords do not match", 400)

        # Insert username in database of not already present
        try:
            db.execute("INSERT INTO users (username, hash) VALUES(?,?)", USERNAME, PASSWORD)
        except ValueError:
            return apology("this username is already registered", 400)

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        SYMBOL = request.form.get("symbol")
        try:
            SHARES_TO_SELL = int(request.form.get("shares"))
            CURR_SHARES = db.execute(
                "SELECT SUM(shares) AS num FROM transactions WHERE symbol=? AND user_id = ?", SYMBOL, session["user_id"])
            CURR_SHARES = CURR_SHARES[0]["num"]
            if not CURR_SHARES or CURR_SHARES == 0:
                return apology("you don't have any of these shares", 400)
            elif CURR_SHARES < SHARES_TO_SELL:
                return apology("exceeding number of shares", 400)
        except ValueError:
            return apology("invalid share", 400)

        # update transactions database
        QUOTED = lookup(SYMBOL)
        PRICE = int(QUOTED["price"]) * SHARES_TO_SELL
        db.execute("INSERT INTO transactions (date_time, symbol, shares, amount, user_id) VALUES(?,?,?,?,?)", datetime.datetime.now(
            pytz.timezone("US/Eastern")).strftime("%Y/%m/%d, %H:%M:%S"), SYMBOL.upper(), -SHARES_TO_SELL, PRICE, session["user_id"])
        # update user database with actual available cash
        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", PRICE, session["user_id"])
        return redirect("/")
    else:
        SYMBOLS = db.execute(
            "SELECT DISTINCT symbol from transactions WHERE user_id=?", session["user_id"])
        return render_template("sell.html", symbols=SYMBOLS)
