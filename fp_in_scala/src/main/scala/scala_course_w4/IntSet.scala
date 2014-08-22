package scala_course_w4

import sun.reflect.generics.reflectiveObjects.NotImplementedException

/**
 * Created by huan on 8/13/14.
 */
abstract class IntSet {
  def incl(x: Int): IntSet
  def contains(x: Int): Boolean
  def union(other: IntSet): IntSet
}

object Empty extends IntSet {
  override def contains(x: Int): Boolean = false
  override def incl(x: Int): IntSet = new NonEmpty(x, Empty, Empty)
  override def union(other: IntSet): IntSet = other
}

class NonEmpty(value: Int, left: IntSet, right: IntSet) extends IntSet {
  override def contains(x: Int): Boolean =
    if (x == value)
      true
    else if (x < value)
      left contains x
    else
      right contains x

  override def incl(x: Int): IntSet =
    if (x == value)
      this
    else if (x < value)
      new NonEmpty(value, left.incl(x), right)
    else
      new NonEmpty(value, left, right.incl(x))

  override def union(other: IntSet): IntSet = ((left union right) union other) incl value
}

object IntSetOps {
  def assertAllPos[S <: IntSet](r: S): S =
    throw new NotImplementedException()
}