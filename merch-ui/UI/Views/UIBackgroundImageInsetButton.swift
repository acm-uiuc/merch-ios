//
//  UIBackgroundImageInsetButton.swift
//  merch-ui
//
//  Created by Rauhul Varma on 3/25/17.
//  Copyright © 2017 acm. All rights reserved.
//

import UIKit

class UIBackgroundImageInsetButton: UIButton {
    override func backgroundRect(forBounds bounds: CGRect) -> CGRect {
        return bounds.insetBy(dx: 24, dy: 18)
    }
}
