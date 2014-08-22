package scala_course_w4

import java.util.NoSuchElementException

/**
 * Created by huan on 8/13/14.
 */
class Cons[T] (val head: T, val tail: List[T]) extends List[T] {
  def isEmpty: Boolean = false
}

object Nil extends List[Nothing] {
  def isEmpty: Boolean = true
  def head: Nothing = throw new NoSuchElementException("Nil.head")
  def tail: Nothing = throw new NoSuchElementException("Nil.tail")
}

class Factory {
  def createOne[T](value: T): List[T] = new Cons[T](value, Nil)
}

class ListOps {
  def nth[T](l: List[T], n: Int): T = {
    def nth_pos_index[T](l: List[T], n: Int): T =
      if (l.isEmpty) throw new IndexOutOfBoundsException()
      else if (n == 0) l.head
      else nth(l.tail, n - 1)

    if (n < 0) throw new IndexOutOfBoundsException()
    else nth_pos_index(l, n)
  }
}

object L {
  def apply[T](x1: T, x2: T): List[T] = new Cons(x1, new Cons(x2, Nil))
  def apply[T](x: T): List[T] = new Cons(x, Nil)
  def apply[T](): List[T] = Nil

  def nth[T](l: List[T], n: Int): T = (new ListOps).nth(l, n)
}