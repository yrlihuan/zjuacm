package scala_course_w4

/**
 * Created by huan on 8/14/14.
 */
trait Expr {
  def eval: Int = this match {
    case Num(n) => n
    case Sum(left, right) => left.eval + right.eval
    case Prod(left, right) => left.eval * right.eval
  }

  def show: String = this match {
    case Num(n) => n.toString
    case Sum(left, right) => left.show + "+" + right.show
    case Prod(left, right) => {
      val p1 = left match {
        case Num(s) => left.show
        case Sum(l, r) => "(" + left.show + ")"
        case Prod(l, r) => left.show
      }
      val p2 = right match {
        case Num(s) => right.show
        case Sum(l, r) => "(" + right.show + ")"
        case Prod(l, r) => right.show
      }

      p1 + "*" + p2
    }
  }
}

case class Num(value: Int) extends Expr {
}

case class Sum(left: Expr, right: Expr) extends Expr {
}

case class Prod(left: Expr, right: Expr) extends Expr {
}