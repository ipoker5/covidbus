package clases;

import com.fasterxml.jackson.annotation.JsonProperty;

public class Tipo_actuador {
	Integer idtipo_actuador;
	Float valor;
	Integer modo;
	Integer idactuador;
	

	public Tipo_actuador(@JsonProperty("idtipo_actuador")Integer idtipo_actuador,@JsonProperty("valor")Float valor,
			@JsonProperty("modo")Integer modo, @JsonProperty("idactuador")Integer idactuador) {
		super();
		this.idtipo_actuador=idtipo_actuador;
		this.valor = valor;
		this.modo = modo;
		this.idactuador = idactuador;
	}


	public Integer getIdtipo_actuador() {
		return idtipo_actuador;
	}


	public void setIdtipo_actuador(Integer idtipo_actuador) {
		this.idtipo_actuador = idtipo_actuador;
	}


	public Float getValor() {
		return valor;
	}


	public void setValor(Float valor) {
		this.valor = valor;
	}


	public Integer getModo() {
		return modo;
	}


	public void setModo(Integer modo) {
		this.modo = modo;
	}


	public Integer getIdactuador() {
		return idactuador;
	}


	public void setIdactuador(Integer idactuador) {
		this.idactuador = idactuador;
	}


	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + ((idactuador == null) ? 0 : idactuador.hashCode());
		result = prime * result + ((idtipo_actuador == null) ? 0 : idtipo_actuador.hashCode());
		result = prime * result + ((modo == null) ? 0 : modo.hashCode());
		result = prime * result + ((valor == null) ? 0 : valor.hashCode());
		return result;
	}


	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		Tipo_actuador other = (Tipo_actuador) obj;
		if (idactuador == null) {
			if (other.idactuador != null)
				return false;
		} else if (!idactuador.equals(other.idactuador))
			return false;
		if (idtipo_actuador == null) {
			if (other.idtipo_actuador != null)
				return false;
		} else if (!idtipo_actuador.equals(other.idtipo_actuador))
			return false;
		if (modo == null) {
			if (other.modo != null)
				return false;
		} else if (!modo.equals(other.modo))
			return false;
		if (valor == null) {
			if (other.valor != null)
				return false;
		} else if (!valor.equals(other.valor))
			return false;
		return true;
	}


}
