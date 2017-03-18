//
//  MerchUIElements.swift
//  merch-ui
//
//  Created by Rauhul Varma on 3/10/17.
//  Copyright © 2017 acm. All rights reserved.
//

import UIKit

class MUIButton: UIButton {
    var defaultBackgroundColor: UIColor?
    var highlightedBackgroundColor: UIColor?
    
    override var isHighlighted: Bool {
        didSet {
            switch isHighlighted {
            case true:
                backgroundColor = highlightedBackgroundColor
            case false:
                backgroundColor = defaultBackgroundColor
            }
        }
    }
}

class MUICircleView: UIView {
    var filledColor: UIColor?
    var unfilledColor: UIColor?
    
    func fillCircle() {
        UIView.animate(withDuration: 0.25) { 
            self.backgroundColor = self.filledColor
        }
    }
    
    func unfillCircle() {
        UIView.animate(withDuration: 0.25) {
            self.backgroundColor = self.unfilledColor
        }
    }
}
