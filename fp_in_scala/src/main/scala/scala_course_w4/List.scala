package scala_course_w4

/**
 * Created by huan on 8/13/14.
 */
trait List[+T] {
  def isEmpty: Boolean
  def head: T
  def tail: List[T]
}
