package clases;

import com.fasterxml.jackson.annotation.JsonProperty;

public class Usuario {
	Integer idusuario;
	String nombre;
	String contraseña;
	String ciudad;
	
	public Usuario(@JsonProperty("idusuario")Integer idusuario, @JsonProperty("nombre")String nombre,
			@JsonProperty("contraseña")String contraseña, @JsonProperty("ciudad")String ciudad) {
		super();
		this.idusuario = idusuario;
		this.nombre = nombre;
		this.contraseña = contraseña;
		this.ciudad = ciudad;
	}



	public Integer getIdusuario() {
		return idusuario;
	}

	public void setIdusuario(Integer idusuario) {
		this.idusuario = idusuario;
	}

	public String getNombre() {
		return nombre;
	}

	public void setNombre(String nombre) {
		this.nombre = nombre;
	}

	public String getContraseña() {
		return contraseña;
	}

	public void setContraseña(String contraseña) {
		this.contraseña = contraseña;
	}

	public String getCiudad() {
		return ciudad;
	}

	public void setCiudad(String ciudad) {
		this.ciudad = ciudad;
	}



	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + ((ciudad == null) ? 0 : ciudad.hashCode());
		result = prime * result + ((contraseña == null) ? 0 : contraseña.hashCode());
		result = prime * result + ((idusuario == null) ? 0 : idusuario.hashCode());
		result = prime * result + ((nombre == null) ? 0 : nombre.hashCode());
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
		Usuario other = (Usuario) obj;
		if (ciudad == null) {
			if (other.ciudad != null)
				return false;
		} else if (!ciudad.equals(other.ciudad))
			return false;
		if (contraseña == null) {
			if (other.contraseña != null)
				return false;
		} else if (!contraseña.equals(other.contraseña))
			return false;
		if (idusuario == null) {
			if (other.idusuario != null)
				return false;
		} else if (!idusuario.equals(other.idusuario))
			return false;
		if (nombre == null) {
			if (other.nombre != null)
				return false;
		} else if (!nombre.equals(other.nombre))
			return false;
		return true;
	}

	
	
	
}
