package scala_course_w3

import org.scalatest.FunSuite

/**
 * Created by huan on 8/13/14.
 */
class RationalTest extends FunSuite {
  test("Rational zero denom") {
    intercept [IllegalArgumentException] {
      new Rational(1, 0)
    }
  }

  test("Rational constructor single param") {
    val r = new Rational(4)
    assertResult(r.toString())("4/1")
  }

  test("Ratinal gcd constructor") {
    assert(new Rational(2, 4).toString() == "1/2")
    assert(new Rational(4, 2).toString() == "2/1")
    assert(new Rational(-4, 2).toString() == "-2/1")
    assert(new Rational(0, 10).toString() == "0/1")
  }

  test("Rational neg") {
    assert((-(new Rational(2, 1))).toString() == "-2/1")
  }

  test("Rational add") {
    val r1 = new Rational(1, 2)
    val r2 = new Rational(2, 4)

    val r = r1 + r2
    assert(r.toString() == "1/1")
  }

  test("Rational sub") {
    val r1 = new Rational(1, 2)
    val r2 = new Rational(2, 4)

    val r = r1 - r2
    assert(r.toString() == "0/1")
  }

  test("Rational mul") {
    var r1 = new Rational(2, 3)
    var r2 = new Rational(3, 4)

    var r = r1 * r2
    assert(r.toString() == "1/2")

    r1 = new Rational(0, 1)
    r2 = new Rational(2, 3)
    r = r1 * r2
    assert(r.toString() == "0/1")
  }
}
