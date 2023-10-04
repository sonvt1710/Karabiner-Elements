import Combine
import SwiftUI

public class StickManager: ObservableObject {
  public static let shared = StickManager()

  struct Event {
    var timeStamp: Date
    var value: Double
    var acceleration: Double

    func attenuatedAcceleration(_ now: Date) -> Double {
      let attenuation = 5 * now.timeIntervalSince(timeStamp)
      if abs(acceleration) < attenuation {
        return 0
      }
      if acceleration > 0 {
        return acceleration - attenuation
      } else {
        return acceleration + attenuation
      }
    }
  }

  class StickSensor: ObservableObject {
    @Published var lastDoubleValue = 0.0
    @Published var lastTimeStamp = Date()
    @Published var lastInterval = 0.0
    @Published var lastAcceleration = 0.0
    var eventHistories: [Event] = []

    @MainActor
    func add(
      _ logicalMax: Int64,
      _ logicalMin: Int64,
      _ integerValue: Int64
    ) {
      if logicalMax != logicalMin {
        let previousDoubleValue = lastDoubleValue
        let previousTimeStamp = lastTimeStamp

        let now = Date()

        // -1.0 ... 1.0
        lastDoubleValue =
          (Double(integerValue - logicalMin) / Double(logicalMax - logicalMin) - 0.5) * 2.0

        lastTimeStamp = now

        lastInterval = max(
          now.timeIntervalSince(previousTimeStamp),
          0.001  // 1 ms
        )

        // -1000.0 ... 1000.0
        let acceleration = (lastDoubleValue - previousDoubleValue) / lastInterval

        eventHistories.append(
          Event(
            timeStamp: now,
            value: lastDoubleValue,
            acceleration: acceleration))
      }
    }

    @MainActor
    func update() {
      guard let last = eventHistories.last else {
        lastAcceleration = 0.0
        return
      }

      let deadzone = 0.1
      if abs(last.value) < deadzone {
        eventHistories.removeAll()
        lastAcceleration = 0.0
      } else {
        let now = Date()
        var sum = 0.0
        eventHistories.forEach { e in
          sum += e.attenuatedAcceleration(now)
        }
        lastAcceleration = sum

        eventHistories.removeAll(where: {
          let attenuatedAcceleration = $0.attenuatedAcceleration(now)
          return attenuatedAcceleration == 0 || sum * attenuatedAcceleration < 0
        })
      }
    }
  }

  class Stick: ObservableObject {
    @Published var horizontal = StickSensor()
    @Published var vertical = StickSensor()
    @Published var radian: Double = 0

    @MainActor
    func update() {
      horizontal.update()
      vertical.update()

      radian = atan2(vertical.lastDoubleValue, horizontal.lastDoubleValue)
    }
  }

  @Published var rightStick = Stick()
}
