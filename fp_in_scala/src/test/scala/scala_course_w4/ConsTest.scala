package scala_course_w4

import org.scalatest.FunSuite

/**
 * Created by huan on 8/13/14.
 */
class ConsTest extends FunSuite {
  test("test factory") {
    val f = new Factory

    val l = f.createOne(1)
    assert(!l.isEmpty)
  }

  test("test nth element") {
    val f = new Factory
    val op = new ListOps
    val l = f.createOne(1)

    assert(op.nth(l, 0) == 1)

    intercept[IndexOutOfBoundsException](op.nth(l, 1))
    intercept[IndexOutOfBoundsException](op.nth(l, -1))
  }

  test("test List singleton") {
    assert(L.nth(L(1), 0) == 1)
    assert(L.nth(L(1, 2), 0) == 1)
    assert(L.nth(L(1, 2), 1) == 2)

    intercept[IndexOutOfBoundsException](L.nth(L(), 0))
  }

  test("test covariance") {
    val x1: List[String] = Nil

  }
}
