package scala_course_w4

import org.scalatest.FunSuite

/**
 * Created by huan on 8/14/14.
 */
class ExprTest extends FunSuite {
  implicit def intWrapper(n: Int): Expr = Num(n)

  test("test adding") {
    val e1: Expr = Sum(1, 2)
    assert(e1.eval == 3)

    val e2: Expr = Sum(Sum(1, 2), Sum(3, 4))
    assert(e2.eval == 10)

    val e3: Expr = Prod(Sum(1, 2), Sum(3, 4))
    assert(e3.eval == 21)

    val e4: Expr = Sum(Prod(1, 2), Prod(3, 4))
    assert(e4.eval == 14)
  }

  test("test show") {
    val e1: Expr = Sum(1, 2)
    assert(e1.show == "1+2")

    val e2: Expr = Sum(Sum(1, 2), Sum(3, 4))
    assert(e2.show == "1+2+3+4")

    val e3: Expr = Sum(5, e2)
    assert(e3.show == "5+1+2+3+4")

    val e4: Expr = Sum(e1, e2)
    assert(e4.show == "1+2+1+2+3+4")

    val e5: Expr = Prod(e1, e2)
    assert(e5.show == "(1+2)*(1+2+3+4)")

    val e6: Expr = Sum(1, Prod(Prod(2, 3), 4))
    assert(e6.show == "1+2*3*4")

    val e7: Expr = Sum(Prod(1, 2), Prod(3, 4))
    assert(e7.show == "1*2+3*4")
  }
}
