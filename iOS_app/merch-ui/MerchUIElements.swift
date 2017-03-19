//
//  MerchUIElements.swift
//  merch-ui
//
//  Created by Rauhul Varma on 3/10/17.
//  Copyright © 2017 acm. All rights reserved.
//

import UIKit

@IBDesignable class MUIButton: UIButton {
    @IBInspectable var defaultBackgroundColor: UIColor?     = nil
    @IBInspectable var highlightedBackgroundColor: UIColor? = nil
    
    @IBInspectable var defaultBorderColor: UIColor? = nil
    @IBInspectable var disabledBorderColor: UIColor? = nil
    
    @IBInspectable var borderWidth: CGFloat {
        set {
            layer.borderWidth = newValue
        }
        get {
            return layer.borderWidth
        }
    }
    @IBInspectable var cornerRadius: CGFloat {
        set {
            layer.cornerRadius = newValue
            clipsToBounds = newValue > 0
        }
        get {
            return layer.cornerRadius
        }
    }

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
    
    override var isEnabled: Bool {
        didSet {
            switch isEnabled {
            case true:
                layer.borderColor = defaultBorderColor?.cgColor
            case false:
                layer.borderColor = disabledBorderColor?.cgColor
            }
        }
    }
}

@IBDesignable class MUICircleView: UIView {
    @IBInspectable var filledColor: UIColor? = nil
    @IBInspectable var emptyColor: UIColor?  = nil
    
    @IBInspectable var borderColor: UIColor? {
        set {
            layer.borderColor = newValue?.cgColor
        }
        get {
            if let color = layer.borderColor {
                return UIColor(cgColor: color)
            }
            else {
                return nil
            }
        }
    }
    @IBInspectable var borderWidth: CGFloat {
        set {
            layer.borderWidth = newValue
        }
        get {
            return layer.borderWidth
        }
    }
    @IBInspectable var cornerRadius: CGFloat {
        set {
            layer.cornerRadius = newValue
            clipsToBounds = newValue > 0
        }
        get {
            return layer.cornerRadius
        }
    }

    
    var filled = false {
        didSet {
            if filled {
                fill()
            } else {
                empty()
            }
        }
    }
    
    private func fill() {
        UIView.animate(withDuration: 0.25) { 
            self.backgroundColor = self.filledColor
        }
    }
    
    private func empty() {
        UIView.animate(withDuration: 0.25) {
            self.backgroundColor = self.emptyColor
        }
    }
}

class MUIHidingLabel: UILabel {
    override var text: String? {
        get {
            return super.text
        }
        set {
            if let newValue = newValue {
                super.text = newValue
                if alpha != 1 {
                    UIView.animate(withDuration: 0.5) {
                        self.alpha = 1
                    }
                }
            } else {
                if alpha != 0 {
                    UIView.animate(withDuration: 0.5, animations: { 
                        self.alpha = 0
                    }) { (_) in
                        super.text = nil
                    }
                } else {
                    super.text = nil
                }
            }
        }
    }
}
