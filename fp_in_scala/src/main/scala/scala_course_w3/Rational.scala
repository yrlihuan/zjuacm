package scala_course_w3

/**
 * Created by huan on 8/13/14.
 */
class Rational(x: Int, y: Int) {
  require(y != 0, "denominator must be non-zero")

  def this(x: Int) = this(x, 1)

  private val g = gcd(x, y)
  def numer = x / g
  def denom = y / g

  def +(other: Rational) =
    new Rational(
      numer * other.denom + other.numer * denom,
      denom * other.denom
    )

  def unary_- : Rational = new Rational(-numer, denom)

  def -(other: Rational): Rational = this + -other

  def *(other: Rational) =
    new Rational(
      numer * other.numer,
      denom * other.denom
    )

  private def gcd(a: Int, b: Int) :Int =
    if (b == 0) a
    else gcd(b, a % b)

  override def toString() = numer + "/" + denom
}
