package scala_course_w4

import org.scalatest.FunSuite

/**
 * Created by huan on 8/13/14.
 */
class IntSetTest extends FunSuite {
  def create_set1: IntSet = {
    var root: IntSet = Empty
    root = root.incl(5)
    root = root.incl(4)
    root = root.incl(1)
    root = root.incl(3)
    root = root.incl(2)
    root.incl(9)
  }

  def create_set2: IntSet = {
    var root: IntSet = Empty
    root = root.incl(2)
    root = root.incl(7)
    root = root.incl(11)
    root = root.incl(13)
    root = root.incl(20)
    root.incl(3)
  }

  test("test adding elem") {
    val s1 = create_set1

    assert(s1.contains(3))
    assert(s1.contains(1))
    assert(s1.contains(9))
    assert(!s1.contains(7))

    val s2 = create_set2

    assert(s2.contains(7))
    assert(s2.contains(13))
    assert(!s2.contains(1))
  }

  test("test union") {
    val s1 = create_set1
    val s2 = create_set2

    val s = s1 union s2

    assert(s.contains(3))
    assert(s.contains(1))
    assert(s.contains(9))
    assert(s.contains(7))
    assert(s.contains(13))
    assert(s.contains(20))
  }
}
